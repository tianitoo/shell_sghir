/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/05/10 17:49:05 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:52:21 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

t_exit	*g_exit = NULL;

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
		i = ft_atoi(tmp->value);
		i++;
		free(tmp->value);
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
	if (add_garbage(data, key_value) == NULL)
		return (NULL);
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
	tmp->next = hidden_path;
}

void	handler2(int arg)
{
	if (g_exit->in_exec_mode != 0)
		return ;
	(void)arg;
	ft_putstr_fd("Quit: 3\n", 1);
	rl_redisplay();
}

void	handler(int arg)
{
	if (g_exit->in_exec_mode != 1)
	{
		(void)arg;
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_exit->g_exit_status = 1;
	}
}

void	set_g_exit(void)
{
	g_exit = malloc(sizeof(t_exit));
	if (!g_exit)
	{
		prompt_error("malloc error 3", NULL, NULL, 1);
		return ;
	}
	g_exit->garbage = NULL;
	g_exit->heredoc_ctrlc = 0;
	g_exit->g_exit_status = 0;
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

t_data	*set_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (prompt_error("malloc error 4", NULL, NULL, 1), NULL);
	data->params = NULL;
	data->linked_env = get_linked_env(envp, data);
	if (!data->linked_env)
		return (NULL);
	add_hidden_env(data->linked_env, "PATH",
		ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."), data);
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	(void)argc;
	(void)argv;
	set_g_exit();
	rl_catch_signals = 0;
	data = set_data(envp);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_exit->in_exec_mode = 0;
		data->parsing_error = 0;
		get_input(data);
		free_garbage();
	}
	return (0);
}


