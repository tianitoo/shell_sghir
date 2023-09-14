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
				return (prompt_error("malloc error", NULL, data, 1), NULL);
			return (key);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_'){
			ft_printf("|%c|\n", str[i]);
			return (prompt_error("minishell: export: not a valid identifier", NULL, data, 1), NULL);}
		i++;
	}
	key = ft_substr(str, 0, i);
	if (key == NULL)
		return (prompt_error("malloc error", NULL, data, 1), NULL);
	return (key);
}

char	*change_var_value(char *new_param, char *key, t_env *tmp, t_data *data)
{
	char	*value;

	if (new_param[ft_strlen(key)] == '+')
	{
		value = get_value(new_param, data);
		if (value == NULL)
			return (NULL);
		if (tmp->value)
			tmp->value = ft_strjoin(tmp->value, value, 1);
		else
		{
			tmp->value = ft_strdup(value);
			if (tmp->value == NULL)
				return (prompt_error("malloc error", NULL, data, 1), NULL);
		}
	}
	else
	{
		free(tmp->value);
		value = get_value(new_param, data);
		if (!value)
			return (NULL);
		tmp->value = value;
	}
	return (value);
}

char	*update_param(t_data *data, char *key, char *new_param)
{
	t_env	*tmp;
	char	*value;
	char	*key_to_find;

	tmp = data->linked_env;
	key_to_find = find_key(new_param, data);
	while (tmp)
	{
		if (ft_strcmp(key_to_find, tmp->key) == 0)
		{
			value = change_var_value(new_param, key, tmp, data);
			if (value == NULL)
				return (NULL);
		}
		tmp = tmp->next;
	}
	free(key_to_find);
	return (value);
}

int	key_exists(t_env *env_params, char *key_to_find)
{
	t_env	*tmp;
	char	*key;

	tmp = env_params;
	while (tmp)
	{
		key = tmp->key;
		if (ft_strcmp(key, key_to_find) == 0 && tmp->hidden == 0)
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
	{
		value = ft_strdup("");
		if (garbage(value, data) == NULL)
			return (prompt_error("malloc error", NULL, data, 1), NULL);
	}
	value = ft_substr(variable, i + 1, ft_strlen(variable));
	if (!value)
		return (prompt_error("malloc error", NULL, data, 1), NULL);
	return (value);
}

int	update_or_add_env(t_data *data, t_params tmp, t_env *linked_env)
{
	char	*key;
	char	*value;

	key = find_key(tmp->parameter, data);
	if (key)
	{
		if (key_exists(data->linked_env, key) == -1)
			return (0);
		else if (key_exists(data->linked_env, key) == 1)
		{
			if (update_param(data, key, tmp->parameter) == NULL)
				return (0);
		}
		else
		{
			value = get_value(tmp->parameter, data);
			while (linked_env && linked_env->next)
				linked_env = linked_env->next;
			linked_env->next = new_env(ft_strdup(key), value, data);
			linked_env->next->exported = 0;
		}
	}
	free(key);
	return (1);
}

t_env	*add_to_env(t_cmd_list cmd_list, t_data *data)
{
	t_env		*linked_env;
	t_params	tmp;

	linked_env = data->linked_env;
	tmp = cmd_list->args->next;
	while (tmp)
	{
		if (tmp->parameter[0] == '=' || ft_isdigit(tmp->parameter[0]) || !(ft_isalpha(tmp->parameter[0]) || tmp->parameter[0] == '_'))
			ft_printf("minishell: export: `%s': not a valid identifier\n", tmp->parameter);
		else if (update_or_add_env(data, tmp, linked_env) == 0)
				return (NULL);
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
