/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:31:42 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 04:44:54 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env(t_env *env)
{
	t_env	*tmp;
	int		i;
	int		j;
	int		len;

	len = env_len(env);
	i = 0;
	tmp = env;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
				move_node(tmp);
			tmp = tmp->next;
			j++;
		}
		tmp = env;
		i++;
	}
}

t_env	*get_env_by_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*set_shlvl(t_env *env, t_data *data)
{
	t_env	*tmp;
	int		i;

	tmp = get_env_by_key(env, "SHLVL");
	if (tmp == NULL)
	{
		if (add_env(env, "SHLVL", "1", data) == NULL)
			return (NULL);
	}
	else
	{
		if (tmp->value[0] == '\0')
			i = 0;
		else
			i = ft_atoi(tmp->value);
		i++;
		free(tmp->value);
		if (i == 1000)
			tmp->value = ft_strdup("");
		else
			tmp->value = ft_itoa(i);
		if (!tmp->value)
			return (NULL);
	}
	return (env);
}

char	**get_key_value(char *envp, t_data *data)
{
	char	**key_value;

	key_value = malloc(sizeof(char *) * 2);
	if (!key_value)
		return (prompt_error("malloc error 8", NULL, data, 1), NULL);
	key_value[0] = find_key(envp, data);
	if (key_value[0] == NULL)
		return (NULL);
	key_value[1] = get_value(envp, data);
	if (key_value[1] == NULL)
		return (NULL);
	return (key_value);
}

t_env	*get_env(char **envp, t_data *data)
{
	t_env	*env;
	t_env	*tmp;
	char	**key_value;
	int		i;

	i = 0;
	while (envp[i])
	{
		key_value = get_key_value(envp[i], data);
		if (i == 0)
		{
			env = new_env(key_value[0], key_value[1], data);
			if (env == NULL)
				return (NULL);
			env->exported = 0;
			tmp = env;
		}
		else
			if (add_env(tmp, key_value[0], key_value[1], data) == NULL)
				return (NULL);
		i++;
	}
	if (set_shlvl(env, data) == NULL)
		return (NULL);
	return (env);
}
