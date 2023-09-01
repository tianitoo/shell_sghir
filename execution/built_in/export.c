/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   export.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 15:54:25 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 15:54:27 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../minishell.h"


char	*find_key(char *str, t_data *data)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
		{
			key = ft_substr(str, 0, i);
			if (key == NULL)
			{
				prompt_error("malloc error", NULL, data, 1);
				return (NULL);
			}
			return (key);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			prompt_error("minishell: export: not a valid identifier", NULL, data, 1);
			return (NULL);
		}
		i++;
	}
	key = ft_substr(str, 0, i);
	if (key == NULL)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);}
	// ft_printf("key = %s\n", key);
	return (key);
}

char	*update_param(t_data *data, char *key, char *new_param)
{
	t_env	*tmp;
	char	*value;
	char	*key_to_find;

	value = get_value(new_param, data);
	if (!value)
		return (NULL);
	// add_garbage(data, value);
	tmp = data->linked_env;
	key_to_find = find_key(new_param, data);
	while (tmp)
	{
		if (ft_strcmp(key_to_find, tmp->key) == 0)
		{
			if (new_param[ft_strlen(key)] == '+')
			{
				if (tmp->value)
					tmp->value = ft_strjoin(tmp->value, value, 1);
				else
				{
					tmp->value = ft_strdup(value);
					if (tmp->value == NULL)
						return (NULL);
				}
			}
			else
			{
				free(tmp->value);
				tmp->value = value;
			}
		}
		tmp = tmp->next;
	}
	return (value);
}

int	key_exists(t_env *env_params, char *key_to_find, t_data *data)
{
	t_env	*tmp;
	char	*key;

	tmp = env_params;
	while (tmp)
	{
		key = find_key(tmp->key, data);
		if (add_garbage(data, key) == NULL)
			return (-1);
		if (ft_strcmp(key, key_to_find) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

char	*get_value(char *variable, t_data *data)
{
	int		i;
	char	*value;

	i = 0;
	while (variable[i] != '=' && variable[i])
		i++;
	if (variable[i] == '\0')
		return (NULL);
	value = ft_substr(variable, i + 1, ft_strlen(variable));
	if (!value)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);}
	return (value);
}

t_env	 *add_to_env(t_cmd_list cmd_list, t_data *data)
{
	// t_env	*linked_env;
	char	*key;
	char	*value;
	t_env	*linked_env;
	t_params	tmp;

	linked_env = data->linked_env;
	tmp = cmd_list->args->next;
	while (tmp)
	{
		if (tmp->parameter[0] == '=' || ft_isdigit(tmp->parameter[0]) || !(ft_isalpha(tmp->parameter[0]) || tmp->parameter[0] == '_'))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", tmp->parameter);
		}
		else
		{
			key = find_key(tmp->parameter, data);
			// add_garbage(data, key);
			if (key)
			{
				if (key_exists(data->linked_env, key, data) == -1)
					return NULL;
				else if (key_exists(data->linked_env, key, data) == 1)
				{
					if (update_param(data, key, tmp->parameter) == NULL)
					return (NULL);
				}
				else
				{
					value = get_value(tmp->parameter, data);
					while (linked_env->next)
						linked_env = linked_env->next;
					linked_env->next = new_env(key, value, data);
					linked_env->next->exported = 1;
				}
			}
		}
		tmp = tmp->next;
	}
	return (data->linked_env);
}

t_env	*ft_export(t_cmd_list cmd_list, t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (cmd_list->args->next == NULL)
	{
		while (env)
		{
			if (env->hidden == 0)
			{
				ft_printf("declare -x %s", env->key);
				if (env->value)
					ft_printf("=\"%s\"\n", env->value);
				else
					ft_printf("\n");
			}
			env = env->next;
		}
	}
	else
		if (add_to_env(cmd_list, data) == NULL)
			return (NULL);
	return (data->linked_env);
}
