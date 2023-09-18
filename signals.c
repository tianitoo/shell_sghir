/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:24:43 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 20:46:06 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	set_g_exit(void)
{
	g_exit = malloc(sizeof(t_exit));
	if (!g_exit)
	{
		prompt_error("malloc error", NULL, NULL, 1);
		return (1);
	}
	g_exit->garbage = NULL;
	g_exit->heredoc_ctrlc = 0;
	g_exit->g_exit_status = 0;
	return (0);
}

void	signalher(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ioctl(0, TIOCSTI, "\4");
	g_exit->heredoc_ctrlc = 1;
}
