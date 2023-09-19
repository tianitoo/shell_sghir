/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:22:17 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:31:28 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_variable(t_env *env, char *var)
{
	t_env	*tmp;
	char	*path;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, var) == 0)
		{
			path = tmp->value;
			return (path);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*new_env(char *key, char *value, t_data *data)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
	{
		prompt_error("malloc error 7", NULL, data, 1);
		return (NULL);
	}
	env->key = key;
	env->value = value;
	env->hidden = 0;
	env->show_value = 0;
	env->unsetable = 0;
	env->next = NULL;
	return (env);
}

t_env	*add_env(t_env *env, char *key, char *value, t_data *data)
{
	t_env	*tmp;

	tmp = env;
	if (tmp == NULL)
	{
		env = new_env(key, value, data);
		if (!env)
			return (NULL);
		return (env);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env(key, value, data);
	if (!tmp->next)
		return (NULL);
	return (env);
}

int	env_len(t_env *env)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	move_node(t_env *tmp)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = tmp->key;
	tmp_value = tmp->value;
	tmp->key = tmp->next->key;
	tmp->value = tmp->next->value;
	tmp->next->key = tmp_key;
	tmp->next->value = tmp_value;
}
