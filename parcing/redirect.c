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
	t_params	prev;
	char	*line;
	int		*pip;

	next = params->next;
	prev = params->prev;
	if (next != NULL)
	{
		if (next->is_operator == 1)
		{
			prompt_error("minishell: syntax error", NULL, data);
			return ;
		}
		pip = (int *)malloc(sizeof(int) * 2);
		pipe(pip);
		line = readline("> ");
		if (next->in_double_quote == -1 && next->in_quote == -1)
		while (strcmp(line, next->parameter) != 0)
		{
			handle_dollar(&line, data);
			write(pip[1], line, ft_strlen(line));
			write(pip[1], "\n", 1);
			free(line);
			line = readline("> ");
		}
		free(line);
		close(pip[1]);
		cmd_list->input = pip[0];
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data);
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
			prompt_error("minishell: syntax", NULL, data);
			return ;
		}
		dir = opendir(next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", next->parameter);
			prompt_error(" ", cmd_list, NULL);
			return ;
		}
		fd = open(next->parameter, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory", next->parameter);
			prompt_error(" ", cmd_list, NULL);
		}
		cmd_list->output = fd;
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data);
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
			prompt_error("minishell: syntax error", NULL, data);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, NULL);
			return ;
		}
		fd = open(params->next->parameter, O_RDONLY);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, NULL);
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
		prompt_error("minishell: syntax error", NULL, data);
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
			prompt_error("minishell: syntax error", NULL, data);
			return ;
		}
		dir = opendir(params->next->parameter);
		if (dir != NULL)
		{
			closedir(dir);
			printf("%s: is a directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, NULL);
			return ;
		}
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("syntax error: could not open file\n");
			prompt_error(" ", cmd_list, NULL);
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
		prompt_error("minishell: syntax error", NULL, data);
}
