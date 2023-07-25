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

void handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data)
{
	if (params->parameter[0] == '<' && ft_strlen(params->parameter) == 1)
		add_input(params, cmd_list, data);
	else if (params->parameter[0] == '>' && ft_strlen(params->parameter) == 1)
		add_output(params, cmd_list, data);
	else if (params->parameter[0] == '<' && params->parameter[1] == '<')
		handle_heredoc(params, cmd_list, data);
	else if (params->parameter[0] == '>' && params->parameter[1] == '>')
		handle_append(params, cmd_list, data);
	// // else if (params->parameter[0] == '<' && params->parameter[1] == '>'
	// // 	|| params->parameter[0] == '>' && params->parameter[1] == '<')
	// // 	prompt_error("minishell: syntax error near unexpected token `newline'");
}

void	handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	next;
	char	*line;
	int		*end;

	next = params->next;
	if (next != NULL)
	{
		if (next->is_operator == 1)
		{
			prompt_error("minishell: syntax error near unexpected token `newline'", data);
			return ;
		}
		end = (int *)malloc(sizeof(int) * 2);
		pipe(end);
		line = readline("> ");
		while (strcmp(line, next->parameter) != 0)
		{
			write(end[1], line, ft_strlen(line));
			write(end[1], "\n", 1);
			free(line);
			line = readline("> ");
		}
		free(line);
		close(end[1]);
		params->prev->next = params->next->next;
		params->next->next->prev = params->prev;
		cmd_list->input = end[0];
	}
	else
		prompt_error("minishell: syntax error near unexpected token `newline'", data);
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
			prompt_error("minishell: syntax error near unexpected token `newline'", data);
			return ;
		}
		dir = opendir(next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", next->parameter);
			return ;
		}
		fd = open(next->parameter, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", next->parameter);
			return ;
		}
		cmd_list->append = fd;
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error near unexpected token `newline'", data);
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
			prompt_error("minishell: syntax error near unexpected token `newline'", data);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			return ;
		}
		fd = open(params->next->parameter, O_RDONLY);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", params->next->parameter);
			return ;
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
		prompt_error("minishell: syntax error near unexpected token `newline'", data);
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
			prompt_error("minishell: syntax error near unexpected token `newline'", data);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			return ;
		}
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", params->next->parameter);
			return ;
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
		prompt_error("minishell: syntax error near unexpected token `newline'", NULL);
}
