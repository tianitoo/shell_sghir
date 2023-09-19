/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:29:53 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 19:20:15 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*add_new_env(char *key, t_params tmp, t_env *linked_env, t_data *data)
{
	char	*value;
	char	*new_key;

	value = get_value(tmp->parameter, data);
	if (value == NULL)
		return (0);
	new_key = ft_strdup(key);
	if (new_key == NULL)
		return (prompt_error("malloc error", NULL, data, 1), NULL);
	while (linked_env && linked_env->next)
		linked_env = linked_env->next;
	linked_env->next = new_env(new_key, value, data);
	linked_env->next->exported = 0;
	if (ft_strchr(tmp->parameter, '=') != NULL)
		linked_env->next->show_value = 1;
	return (key);
}

int	update_or_add_env(t_data *data, t_params tmp, t_env *linked_env)
{
	char	*key;

	key = find_key(tmp->parameter, data);
	if (key)
	{
		if (key_exists_hidden(data->linked_env, key) == -1)
			return (0);
		else if (key_exists_hidden(data->linked_env, key) == 1)
		{
			if (ft_strchr(tmp->parameter, '=') != NULL)
			{
				if (update_param(data, key, tmp->parameter) == NULL)
					return (0);
			}
		}
		else
		{
			if (add_new_env(key, tmp, linked_env, data) == NULL)
				return (0);
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
		if (tmp->parameter[0] == '='
			|| ft_isdigit(tmp->parameter[0])
			|| !(ft_isalpha(tmp->parameter[0]) || tmp->parameter[0] == '_'))
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				tmp->parameter);
		else if (update_or_add_env(data, tmp, linked_env) == 0)
			return (NULL);
		tmp = tmp->next;
	}
	return (data->linked_env);
}
