/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:30:34 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 18:09:49 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_unset_env(void)
{
	char	**env;
	char	*pwd;

	env = (char **) malloc(sizeof(char *) * 4);
	if (!env)
		return (prompt_error("malloc error 5", NULL, NULL, 1), NULL);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (prompt_error("malloc error 6", NULL, NULL, 1), NULL);
	env[0] = ft_strjoin("PWD=", pwd, 0);
	free(pwd);
	env[1] = ft_strdup("SHLVL=1");
	env[2] = ft_strdup("_=/usr/bin/env");
	env[3] = NULL;
	if (!env[0] || !env[1] || !env[2])
		return (prompt_error("malloc error 7", NULL, NULL, 1), NULL);
	return (env);
}

t_env	*get_linked_env(char **envp, t_data *data)
{
	char	**env;
	int		unset;
	t_env	*linked_env;

	unset = 0;
	if (envp[0] == NULL)
	{
		env = get_unset_env();
		if (env == NULL)
			return (NULL);
		unset = 1;
	}
	else
		env = envp;
	linked_env = get_env(env, data);
	if (linked_env == NULL)
		return (NULL);
	if (unset == 1)
	{
		free_ss(env);
		free(env);
	}
	return (linked_env);
}

void	add_hidden_env(t_env *env, char *key, char *value, t_data *data)
{
	t_env	*tmp;
	t_env	*hidden_path;

	if (get_env_by_key(env, key) != NULL)
	{
		free(value);
		return ;
	}
	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	hidden_path = new_env(key, value, data);
	hidden_path->hidden = 1;
	hidden_path->show_value = 0;
	hidden_path->exported = 0;
	hidden_path->unsetable = 0;
	tmp->next = hidden_path;
}

t_env	*update_env_var(char *var_key, char *value, t_data *data)
{
	t_env	*env;

	if (value == NULL)
		return (NULL);
	env = data->linked_env;
	while (env)
	{
		if (ft_strcmp(env->key, var_key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (env->value == NULL)
				return (NULL);
			break ;
		}
		env = env->next;
	}
	return (env);
}

int	key_exists_hidden(t_env *env_params, char *key_to_find)
{
	t_env	*tmp;
	char	*key;

	tmp = env_params;
	while (tmp)
	{
		key = tmp->key;
		if (ft_strcmp(key, key_to_find) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
