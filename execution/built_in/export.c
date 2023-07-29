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
			return (key);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			prompt_error("minishell: export: not a valid identifier", NULL, data);
			return (NULL);
		}
		i++;
	}
	key = ft_substr(str, 0, i);
	// ft_printf("key = %s\n", key);
	return (key);
}

void	update_param(t_data *data, char *key, char *new_param)
{
	t_env	*tmp;
	char	*value;

	value = get_value(new_param);
	if (!value)
		return ;
	tmp = data->linked_env;
	while (tmp)
	{
		if (ft_strcmp(find_key(key, data), tmp->key) == 0)
		{
			if (new_param[ft_strlen(key)] == '+')
			{
				if (tmp->value)
					tmp->value = ft_strjoin(tmp->value, value, 1);
				else
					tmp->value = ft_strdup(value);
			}
			else
			{
				tmp->value = ft_strdup(value);
			}
			add_garbage(tmp->value);
		}
		tmp = tmp->next;
	}
}

int	key_exists(t_env *env_params, char *key, t_data *data)
{
	t_env	*tmp;

	tmp = env_params;
	while (tmp)
	{
		if (ft_strcmp(find_key(key, data), tmp->key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

char	*get_value(char *variable)
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
		return (NULL);
	return (value);
}

void	 add_to_env(t_cmd_list cmd_list, t_data *data)
{
	// t_env	*linked_env;
	char	*key;
	char	*value;
	t_env	*linked_env;
	t_params	tmp;

	linked_env = data->linked_env;
	tmp = cmd_list->args;
	while (tmp)
	{
		key = find_key(tmp->parameter, data);
		if (key_exists(data->linked_env, key, data))
			update_param(data, key, tmp->parameter);
		else
		{
			value = get_value(tmp->parameter);
			while (linked_env->next)
				linked_env = linked_env->next;
			linked_env->next = new_env(key, value);
			linked_env->next->exported = 1;
		}
		tmp = tmp->next;
	}
}

void	ft_export(t_cmd_list cmd_list, t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (cmd_list->args->next == NULL)
	{
		while (env)
		{
			ft_printf("declare -x %s", env->key);
			if (env->value)
				ft_printf("=\"%s\"\n", env->value);
			else
				ft_printf("\n");
			env = env->next;
		}
	}
	else
		add_to_env(cmd_list, data);
}
