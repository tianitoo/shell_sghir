/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:47:47 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 05:15:38 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_exitstate(int wait_status)
{
	if (WIFSIGNALED(wait_status))
	{
		if (WTERMSIG(wait_status) == 3)
			write(2, "Quit: 3\n", 8);
		if (WTERMSIG(wait_status) == 2)
			write(2, "\n", 1);
		return (128 + WTERMSIG(wait_status));
	}
	else if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	return (1);
}

pid_t	execute_commands(t_data *data)
{
	t_cmd_list	cmd_list;
	pid_t		pid;

	cmd_list = init_pipes(data);
	while (cmd_list)
	{
		if (cmd_list->parsing_error == 0 && data->parsing_error == 0)
		{
			if (is_builtin(cmd_list->cmd) && cmd_list->next == NULL
				&& cmd_list->prev == NULL)
			{
				if (execute_builtin(data, cmd_list) == NULL)
					break ;
			}
			else if (cmd_list->cmd)
			{
				pid = execute_cmd(data, cmd_list);
				if (pid == -2)
					break ;
			}
		}
		cmd_list = cmd_list->prev;
	}
	return (pid);
}

void	execute(t_data *data)
{
	pid_t		pid;

	pid = execute_commands(data);
	close_file_descriptors(data);
	waitpid(pid, &g_exit->g_exit_status, 0);
	while (waitpid(-1, NULL, 0) != -1)
		;
	g_exit->g_exit_status = get_exitstate(g_exit->g_exit_status);
}

int	execute_cmd(t_data *data, t_cmd_list cmd_list)
{
	pid_t		pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (is_builtin(cmd_list->cmd))
		{
			pipes_work(cmd_list);
			execute_builtin(data, cmd_list);
			exit(0);
		}
		set_up_execve(cmd_list, data);
		exit(1);
	}
	else if (pid < 0)
		prompt_error("Error: fork failed", NULL, data, 1);
	return (pid);
}
