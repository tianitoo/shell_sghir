/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:22:28 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 02:55:23 by hnait            ###   ########.fr       */
/*                                                                            */
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
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (prompt_error("minishell: export: not a valid identifier",
					NULL, data, 1), NULL);
		i++;
	}
	key = ft_substr(str, 0, i);
	if (key == NULL)
		return (prompt_error("malloc error", NULL, data, 1), NULL);
	return (key);
}

char	*update_value(char *new_param, t_env *tmp, t_data *data)
{
	char	*value;

	free(tmp->value);
	value = get_value(new_param, data);
	if (!value)
		return (NULL);
	tmp->value = value;
	tmp->hidden = 0;
	return (value);
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
				return (prompt_error("malloc error", NULL, data, 1),
					free(value), NULL);
		}
	}
	else
		value = update_value(new_param, tmp, data);
	free(value);
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
				return (NULL);
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
