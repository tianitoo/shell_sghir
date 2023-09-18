/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:22:51 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:56:33 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*read_line_heredoc(t_data *data, int pip, char *line)
{
	write(pip, line, ft_strlen(line));
	write(pip, "\n", 1);
	line = readline("> ");
	if (add_garbage(data, line) == NULL)
		return (NULL);
	return (line);
}

void	child_process(t_data *data, int *pip, t_params next)
{
	char	*line;

	g_exit->in_exec_mode = 0;
	add_history(data->original_commande_line);
	signal(SIGINT, signalher);
	line = readline("> ");
	if (garbage(line, data) == NULL)
		exit (1);
	while (ft_strcmp(line, next->parameter) != 0 && line != NULL)
	{
		if (next->in_double_quote == -1 && next->in_quote == -1)
			if (handle_dollar(&line, data) == NULL)
				exit (1);
		line = read_line_heredoc(data, pip[1], line);
		g_exit->number_of_lines++;
	}
	empty_pipe(line, pip);
	close(pip[0]);
	close(pip[1]);
	free_garbage();
	free_params(&data->params);
	rl_clear_history();
	exit(0);
}

int	create_heredoc_process(t_data *data,
	t_cmd_list cmd_list, t_params params)
{
	int		*pip;
	int		pid;

	if (params->next == NULL)
		return (prompt_error("minishell: syntax error", NULL, data, 258), 0);
	pip = create_heredoc_pipe(params->next, data);
	if (pip == NULL)
		return (0);
	pid = fork();
	if (pid == -1)
		return (prompt_error("minishell: fork error", NULL, data, 1), 0);
	else if (pid == 0)
		child_process(data, pip, params->next);
	else
	{
		waitpid(pid, &g_exit->g_exit_status, 0);
		close(pip[1]);
		if (cmd_list->input != -1)
			close(cmd_list->input);
		cmd_list->input = pip[0];
		skip_riderection(params, cmd_list);
	}
	return (1);
}

int	handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data)
{
	if (create_heredoc_process(data, cmd_list, params) == 0)
		return (0);
	return (1);
}
