/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   redirect.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/02 10:39:31 by hnait			 #+#	#+#			 */
/*   Updated: 2023/06/02 13:49:36 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int is_redirect_operator(t_params param)
{
	if (ft_strncmp(param->parameter, ">", 2) == 0
		|| ft_strncmp(param->parameter, "<", 2) == 0
		|| ft_strncmp(param->parameter, ">>", 3) == 0
		|| ft_strncmp(param->parameter, "<<", 3) == 0)
		return (1);
	return (0);
}

int handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data)
{
	if (params->parameter[0] == '<' && ft_strlen(params->parameter) == 1)
		add_input(params, cmd_list, data);
	else if (params->parameter[0] == '>' && ft_strlen(params->parameter) == 1)
		add_output(params, cmd_list, data);
	else if (params->parameter[0] == '<' && params->parameter[1] == '<')
	{
		if (handle_heredoc(params, cmd_list, data) == 0)
			return (0);
	}
	else if (params->parameter[0] == '>' && params->parameter[1] == '>')
		handle_append(params, cmd_list, data);
	// // else if (params->parameter[0] == '<' && params->parameter[1] == '>'
	// // 	|| params->parameter[0] == '>' && params->parameter[1] == '<')
	// // 	prompt_error("minishell: syntax error near unexpected token `newline'");
	return (1);
}

int	make_history(t_data *data, int history_pipe)
{
	int eof;
	char *history;

	history = ft_calloc(sizeof(char), 2);
	if (!history)
		return (prompt_error("minishell: malloc error", NULL, data, 1), 0);
	if (add_garbage(data, history) == NULL)
		return (0);
	eof = read(history_pipe, history, 1);
	data->commande_line = ft_strjoin_char(data->commande_line, '\n', data);
	if (data->commande_line == NULL|| add_garbage(data, data->commande_line) == NULL)
		return (0);
	while (eof > 0)
	{
		data->commande_line = ft_strjoin_char(data->commande_line, history[0], data);
		if (data->commande_line == NULL)
			return (0);
		if (add_garbage(data, data->commande_line) == NULL)
			return (0);
		if (data->commande_line == NULL)
			return (0);
		eof = read(history_pipe, history, 1);
	}
	return (1);
}

int	handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	next;
	t_params	prev;
	char	*line;
	int		*pip;
	int		*history_pipe;
	int		pid;

	history_pipe = (int *)malloc(sizeof(int) * 2); // tested
	if (!history_pipe)
		return (prompt_error("minishell: malloc error", NULL, data, 1), 0);
	if (add_garbage(data, history_pipe) == NULL)
		return (0);
	pipe(history_pipe);
	next = params->next;
	prev = params->prev;
	if (next != NULL)
	{
		if (next->is_operator == 1)
			return (prompt_error("minishell: syntax error", NULL, data, 258), 0);
		pip = (int *)malloc(sizeof(int) * 2); // tested
		if (!pip)
			return (prompt_error("minishell: malloc error", NULL, data, 1), 0);
		if (add_garbage(data, pip) == NULL)
			return (0);
		pipe(pip);
		pid = fork();
		if (pid == 0)
		{
			add_history(data->commande_line);
			close(pip[0]);
			close(history_pipe[0]);
			line = readline("> ");
			if (add_garbage(data, line) == NULL)
				return (0);
			while (ft_strcmp(line, next->parameter) != 0 && line != NULL)
			{
				add_history(line);
				if (next->in_double_quote == -1 && next->in_quote == -1)
				{
					if (handle_dollar(&line, data) == NULL)
						return (0);
				}
				write(history_pipe[1], line, ft_strlen(line));
				write(history_pipe[1], "\n", 1);
				write(pip[1], line, ft_strlen(line));
				write(pip[1], "\n", 1);
				line = readline("> ");
				if (add_garbage(data, line) == NULL)
					return (0);
			}
			close(pip[1]);
			close(history_pipe[1]);
			free_garbage();
			free_params(&data->params);
			// rl_clear_history();
			exit(0);
		}
		else
		{
			waitpid(pid, &g_exit->g_exit_status, 0);
			close(pip[1]);
			close(history_pipe[1]);
			cmd_list->input = pip[0];
			if (prev)
				params->prev->next = next->next;
			if (prev == NULL)
				cmd_list->args = next->next;
			if (next->next)
				next->next->prev = prev;
		}
	}
	else
		return (prompt_error("minishell: syntax error", NULL, data, 258), 0);
	if (make_history(data, history_pipe[0]) == 0)
		return (0);
	return (1);
}

void	handle_append(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	next;
	t_params	prev;
	DIR			*dir;
	int			fd;

	next = params->next;
	prev = params->prev;
	if (next != NULL)
	{
		if (next->is_operator == 1)
		{
			prompt_error("minishell: syntax error", NULL, data, 258);
			return ;
		}
		dir = opendir(next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", next->parameter);
			prompt_error(" ", cmd_list, data, 1);
			return ;
		}
		fd = open(next->parameter, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			prompt_error("syntax error: could not open file", NULL, data, 1);
		cmd_list->output = fd;
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
}

void	add_input(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	prev;
	t_params	next;
	int			fd;
	DIR			*dir;

	prev = params->prev;
	next = params->next;
	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
		{
			prompt_error("minishell: syntax error", NULL, data, 258);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, data, 1);
			return ;
		}
		fd = open(params->next->parameter, O_RDONLY);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, data, 1);
		}
		cmd_list->input = fd;
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
}

void	add_output(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	prev;
	t_params	next;
	int			fd;
	DIR			*dir;

	prev = params->prev;
	next = params->next;
	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
		{
			prompt_error("minishell: syntax error", NULL, data, 258);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, data, 1);
			return ;
		}
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("syntax error: could not open file\n");
			prompt_error(" ", cmd_list, data, 1);
		}
		cmd_list->output = fd;
		prev = params->prev;
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
}
