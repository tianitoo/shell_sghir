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

int	g_exit_status = 0;

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

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 1;
		// rl_on_new_line();
		write(1, "\n", 1);
		// ft_printf("helloo\n");
		// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
}

void	sigquit_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		g_exit_status = 131;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		// rl_replace_line("jskdjfkj", 0);
		// rl_on_new_line();
	}
}
void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		// rl_replace_line("f", 0);
		// rl_on_new_line();
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	(void)argc;
	(void)argv;
	// (void)envp;
	// data->env = (char **) malloc(sizeof(char *) * sizeof(envp));
	data = malloc(sizeof(t_data));
	data->env = envp;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint);
		get_input(data);
	}
	return (0);
}
