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

// void	handle_sigint(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		g_exit_status = 1;
		
// 		write(1, "\n", 1);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 		// ft_printf("helloo\n");
// 		// ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	}
// }

// void	sigquit_handler(int sig)
// {
// 	if (sig == SIGQUIT)
// 	{
// 		g_exit_status = 131;
// 		ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		// rl_replace_line("jskdjfkj", 0);
// 		// rl_on_new_line();
// 	}
// }
// int a = 0;
// void	heredoc_sigint_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		g_exit_status = 1;
// 		ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		// rl_replace_line("f", 0);
// 		rl_on_new_line();
// 	}
// }

t_env	*new_env(char *key, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}

void	add_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;

	tmp = env;
	if (tmp == NULL)
	{
		env = new_env(key, value);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env(key, value);
}

// sort env alphabetically by key

void	sort_env(t_env *env)
{
	t_env	*tmp;
	int		i;
	int		j;
	int		len;

	len = 0;
	i = 0;
	tmp = env;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	tmp = env;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
			{
				char *tmp_key;
				char *tmp_value;
				tmp_key = tmp->key;
				tmp_value = tmp->value;
				tmp->key = tmp->next->key;
				tmp->value = tmp->next->value;
				tmp->next->key = tmp_key;
				tmp->next->value = tmp_value;
			}
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

t_env	*get_env(char **envp)
{
	t_env	*env;
	t_env	*tmp;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_substr(envp[i], ft_strchr(envp[i], '=') - envp[i] + 1,
				ft_strlen(envp[i]) - ft_strlen(key));
		if (i == 0)
		{
			env = new_env(key, value);
			env->exported = 0;
			tmp = env;
		}
		else
			add_env(tmp, key, value);
		i++;
	}
	tmp = get_env_by_key(env, "SHLVL");
	i = ft_atoi(tmp->value);
	i++;
	free(tmp->value);
	tmp->value = ft_itoa(i);
	// sort_env(env);
	return (env);
}

char	**get_unset_env(void)
{
	char	**env;
	char	*pwd;

	env = (char **) malloc(sizeof(char *) * 4);
	pwd = getcwd(NULL, 0);
	env[0] = ft_strjoin("PWD=", pwd, 0);
	env[1] = ft_strdup("SHLVL=1");
	env[2] = ft_strdup("_=/usr/bin/env");
	env[3] = NULL;
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;
	char		**env;

	(void)argc;
	(void)argv;
	g_exit = malloc(sizeof(t_exit));
	g_exit->g_exit_status = 0;
	data = malloc(sizeof(t_data));
	if (envp[0] == NULL)
		env = get_unset_env();
	else
		env = envp;
	data->linked_env = get_env(env);
	while (1)
	{
		// signal(SIGQUIT, SIG_IGN);No such file or directory
		// signal(SIGINT, handle_sigint);
		data->parsing_error = 0;
		get_input(data);
		free_garbage();
	}
	return (0);
}
