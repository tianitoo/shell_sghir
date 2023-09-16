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

int	is_redirect_operator(t_params param)
{
	if (ft_strncmp(param->parameter, ">", 2) == 0
		|| ft_strncmp(param->parameter, "<", 2) == 0
		|| ft_strncmp(param->parameter, ">>", 3) == 0
		|| ft_strncmp(param->parameter, "<<", 3) == 0)
		return (1);
	return (0);
}

int	handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data)
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
	return (1);
}

char	*read_line_heredoc(t_data *data, int pip, char *line)
{
	write(pip, line, ft_strlen(line));
	write(pip, "\n", 1);
	line = readline("> ");
	if (add_garbage(data, line) == NULL)
		return (NULL);
	return (line);
}

int	*create_heredoc_pipe(t_params next, t_data *data)
{
	int	*pip;
	int	pip_test;

	if (next->is_operator == 1)
		return (prompt_error("minishell: syntax error", NULL, data, 258), NULL);
	pip = (int *)malloc(sizeof(int) * 2);
	if (!pip)
		return (prompt_error("minishell: malloc error", NULL, data, 1), NULL);
	if (add_garbage(data, pip) == NULL)
		return (NULL);
	pip_test = pipe(pip);
	if (pip_test == -1)
		return (prompt_error("minishell: pipe error", NULL, data, 1), NULL);
	return (pip);
}

void	signalher(int sig)
{
	(void)sig;
	// exit(0);
	rl_replace_line("", 0);
	ioctl(0, TIOCSTI, "\4");
}

void	child_process(t_data *data, int *pip, t_params next)
{
    char *line;

    add_history(data->original_commande_line);
    close(pip[0]);
    signal(SIGINT, signalher);
    line = readline("> ");
    if (add_garbage(data, line) == NULL)
        exit (1);
    while (ft_strcmp(line, next->parameter) != 0 && line != NULL)
    {
        // add_history(line);
        if (next->in_double_quote == -1 && next->in_quote == -1)
            if (handle_dollar(&line, data) == NULL)
                exit (1);
        line = read_line_heredoc(data, pip[1], line);
    }
    close(pip[1]);
    free_garbage();
    free_params(&data->params);
    rl_clear_history();
    exit(0);
}

void	skip_riderection(t_params params, t_cmd_list cmd_list)
{
	if (params->prev)
		params->prev->next = params->next->next;
	if (params->prev == NULL)
		cmd_list->args = params->next->next;
	if (params->next->next)
		params->next->next->prev = params->prev;
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
		close(pip[0]);
		close(pip[1]);
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

t_params	handle_append(t_params params, t_cmd_list cmd_list, t_data *data)
{
	DIR			*dir;
	int			fd;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
			return (ft_printf("minishell: syntax error near unexpected token `%s'" , params->next->parameter), prompt_error("",
					NULL, data, 258), NULL);
		dir = opendir(params->next->parameter);
		if (dir != NULL)
			return (closedir(dir), ft_printf("%s: is a directory\n",
					params->next->parameter), prompt_error(" ",
					cmd_list, data, 1), NULL);
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			return (prompt_error("minishell: no such file or directory",
					cmd_list, NULL, 1), NULL);
		cmd_list->output = fd;
		skip_riderection(params, cmd_list);
	}
	else
		return (prompt_error("minishell: syntax error", NULL, data, 258), NULL);
	return (params);
}

t_params	add_input(t_params params, t_cmd_list cmd_list, t_data *data)
{
	int			fd;
	DIR			*dir;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
			return (prompt_error("minishell: syntax error",
					NULL, data, 258), NULL);
		dir = opendir(params->next->parameter);
		if (dir != NULL)
			return (closedir(dir), ft_printf("%s: is a directory\n",
					params->next->parameter), prompt_error(" ",
					cmd_list, data, 1), NULL);
		fd = open(params->next->parameter, O_RDONLY);
		if (fd == -1)
			return (ft_printf("minishell: %s: no such file or directory", params->next->parameter), prompt_error("",
					cmd_list, NULL, 1), NULL);
		cmd_list->input = fd;
		skip_riderection(params, cmd_list);
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
	return (params);
}

t_params	add_output(t_params params, t_cmd_list cmd_list, t_data *data)
{
	int			fd;
	DIR			*dir;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
			return (prompt_error("minishell: syntax error",
					NULL, data, 258), NULL);
		dir = opendir(params->next->parameter);
		if (dir != NULL)
			return (closedir(dir), ft_printf("%s: is a directory\n",
					params->next->parameter), prompt_error(" ",
					cmd_list, data, 1), NULL);
		fd = open(params->next->parameter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_printf("minishell: %s: no such file or directory", params->next->parameter), prompt_error("",
					cmd_list, NULL, 1), NULL);
		cmd_list->output = fd;
		skip_riderection(params, cmd_list);
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
	return (params);
}
