/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:22:28 by hnait             #+#    #+#             */
/*   Updated: 2023/09/20 00:13:18 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*update_value(char *new_param, t_data *data)
{
	char	*value;

	value = get_value(new_param, data);
	if (!value)
		return (NULL);
	return (value);
}

char	*append_value(char *new_param, t_env *tmp, t_data *data)
{
	char	*value;

	value = get_value(new_param, data);
	if (value == NULL)
		return (NULL);
	if (tmp->value)
		tmp->value = ft_strjoin(tmp->value, value, 1);
	else
	{
		tmp->value = ft_strdup(value);
		if (tmp->value == NULL)
			return (prompt_error("malloc error", NULL, data, 1),
				free(value), NULL);
	}
	return (value);
}

char	*change_var_value(char *new_param, char *key, t_env *tmp, t_data *data)
{
	char	*value;

	if (new_param[ft_strlen(key)] == '+')
		value = append_value(new_param, tmp, data);
	else
	{
		value = get_value(new_param, data);
		if (value == NULL)
			return (NULL);
		free(tmp->value);
		tmp->value = ft_strdup(value);
	}
	free(value);
	value = NULL;
	return (tmp->value);
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
				return (free(key_to_find), NULL);
			tmp->show_value = 1;
		}
		tmp = tmp->next;
	}
	free(key_to_find);
	return (value);
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
				if (env->value != NULL && ft_strlen(env->value) > 0)
					ft_printf("=\"%s\"\n", env->value);
				else if (env->show_value)
					ft_printf("=\"\"\n");
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
