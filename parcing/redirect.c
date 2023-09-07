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
			int fd[2];

	fd[0] = dup(0);
	fd[1] = dup(1);
		if (handle_heredoc(params, cmd_list, data) == 0)
		{
			dup2(fd[0], 0);
			dup2(fd[1], 1);
			g_exit->closeed = 1;
			signal(SIGINT, handler);
			return (0);
		}
		else
		{
			g_exit->closeed = 1;
			signal(SIGINT, handler);
		}
		dup2(fd[0], 0);
		dup2(fd[1], 1);
	}
	else if (params->parameter[0] == '>' && params->parameter[1] == '>')
		handle_append(params, cmd_list, data);
	// // else if (params->parameter[0] == '<' && params->parameter[1] == '>'
	// // 	|| params->parameter[0] == '>' && params->parameter[1] == '<')
	// // 	prompt_error("minishell: syntax error near unexpected token `newline'");
	return (1);
}

void	handler_X(int sg)
{
	g_exit->closeed = 0;
	close(0);
	(void) sg;
}

int	handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data)
{
	t_params	next;
	t_params	prev;
	char	*line;
	int		*pip;

	next = params->next;
	prev = params->prev;
	g_exit->closeed = 1;
	signal(SIGINT, handler_X);
	if (next != NULL)
	{  
		write(2, "lining!\n", strlen("lining!\n"));
		if (next->is_operator == 1)
		{
			prompt_error("minishell: syntax error", NULL, data, 258);
			return (0);
		}
		pip = (int *)malloc(sizeof(int) * 2); // tested
		if (!pip)
		{
			prompt_error("minishell: malloc error", NULL, data, 1);
			return (0);
		}
		if (add_garbage(data, pip) == NULL)
			return (0);
		pipe(pip);
		line = readline("> ");
		if (add_garbage(data, line) == NULL)
			return (0);
		while (ft_strcmp(line, next->parameter) != 0 && g_exit->closeed)
		{
			write(2, next->parameter, strlen(next->parameter));
			write(2, "\n", strlen("\n"));
			write(2, line, strlen(line));
			write(2, "\n", strlen("\n"));
			if (next->in_double_quote == -1 && next->in_quote == -1)
			{
				if (handle_dollar(&line, data) == NULL)
					return (0);
			}
			write(pip[1], line, ft_strlen(line));
			write(pip[1], "\n", 1);
			line = readline("> ");
			if (line == NULL) {
				break;
			}
			if (add_garbage(data, line) == NULL)
				return (0);
		}
		close(pip[1]);
		if (g_exit->closeed == 0)
			return (0);
		// if(g_exit->heredoc_statu == 1)
		// 	return(0);
		cmd_list->input = pip[0];
		if (prev)
			params->prev->next = next->next;
		if (prev == NULL)
			cmd_list->args = next->next;
		if (next->next)
			next->next->prev = prev;
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
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
			prompt_error(" ", cmd_list, NULL, 1);
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
			prompt_error(" ", cmd_list, NULL, 1);
			return ;
		}
		fd = open(params->next->parameter, O_RDONLY);
		if (fd == -1)
		{
			ft_printf("%s: no such file or directory\n", params->next->parameter);
			prompt_error(" ", cmd_list, NULL, 1);
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
			prompt_error(" ", cmd_list, NULL, 1);
			return ;
		}
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("syntax error: could not open file\n");
			prompt_error(" ", cmd_list, NULL, 1);
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
