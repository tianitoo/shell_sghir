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


char	*get_variable(char **envp, char *var)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
		{
			path = ft_substr(envp[i], ft_strlen(var) + 1,
					ft_strlen(envp[i]) - ft_strlen(var));
			
			return (path);
		}
		i++;
	}
	return (NULL);
}

// void	handle_sigint(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		g_exit_status = 1;
// 		// rl_on_new_line();
// 		write(1, "\n", 1);
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
// void	heredoc_sigint_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		g_exit_status = 1;
// 		ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		// rl_replace_line("f", 0);
// 		// rl_on_new_line();
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
	sort_env(env);
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	(void)argc;
	(void)argv;
	// (void)envp;
	// data->env = (char **) malloc(sizeof(char *) * sizeof(envp));
	data = malloc(sizeof(t_data));
	data->linked_env = get_env(envp);
	data->env = envp;
	while (1)
	{
		// signal(SIGQUIT, SIG_IGN);
		// signal(SIGINT, handle_sigint);
		get_input(data);
	}
	return (0);
}
