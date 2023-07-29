/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   execute.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 14:04:42 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 16:26:04 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_data *data, t_cmd_list cmd_list)
{
	int	input;
	int	output;

	if (cmd_list->input != -1)
	{
		input = dup(0);
		dup2(cmd_list->input, 0);
		close(cmd_list->input);
	}
	if (cmd_list->output != -1)
	{
		output = dup(1);
		dup2(cmd_list->output, 1);
		close(cmd_list->output);
	}
	pipes_work(cmd_list);
	if (ft_strcmp(cmd_list->cmd, "echo") == 0)
		ft_echo(cmd_list->args);
	else if (ft_strcmp(cmd_list->cmd, "cd") == 0)
		ft_cd(cmd_list->args, data);
	else if (ft_strcmp(cmd_list->cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd_list->cmd, "export") == 0)
		ft_export(cmd_list, data);
	else if (ft_strcmp(cmd_list->cmd, "unset") == 0)
		ft_unset(data, cmd_list);
	else if (ft_strcmp(cmd_list->cmd, "env") == 0)
		write_env(data);
	else if (ft_strcmp(cmd_list->cmd, "exit") == 0)
		ft_exit(data);
	if (cmd_list->input != -1)
	{
		dup2(input, 0);
		close(input);
	}
	if (cmd_list->output != -1)
	{
		dup2(output, 1);
		close(output);
	}
}

char *get_cmd_path_from_paths(char **paths, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	*tmp;
	DIR		*dir;

	i = 0;
	if (cmd[0] == '/' || cmd[0] == '.')
		return (cmd);
	if (paths == NULL)
	{
		ft_printf("Error: %s: no such file or directory\n", cmd);
		exit(1);
	}
	if (ft_strlen(cmd) == 0)
	{
		ft_putstr_fd("Error: command not found\n", 2);
		exit(1);
	}
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0);
		// add_garbagee(tmp);
		cmd_path = ft_strjoin(tmp, cmd, 1);
		add_garbage(cmd_path);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
			return (cmd_path);
		i++;
	}
	dir = opendir(cmd);
	if (dir != NULL)
	{
		ft_printf("Error: %s: is a directory\n", cmd);
		(void)closedir(dir);
		exit(1);
	}
	ft_printf("Error: %s: command not found\n", cmd);
	exit(1);
}

char	*get_cmd_path(t_data *data, t_cmd_list cmd_list)
{
	char	*cmd;
	char	*path;
	char	**paths;
	t_env	*env;

	env = data->linked_env;
	path = get_variable(env, "PATH");
	if (path != NULL)
		paths = ft_split(path, ':');
	else
		paths = NULL;
	cmd = get_cmd_path_from_paths(paths, cmd_list->cmd);
	if (cmd == NULL)
	{
		ft_putstr_fd("Error: command not found\n", 2);
		return (NULL);
	}
	free_ss(paths);
	return (cmd);
}

void	pipes_work(t_cmd_list cmd_list)
{
	if (cmd_list->next != NULL && cmd_list->output == -1)
		dup2(cmd_list->next->pip[1], 1);
	else if (cmd_list->output != -1)
	{
		dup2(cmd_list->output, 1);
		close(cmd_list->output);
	}
	if (cmd_list->input == -1 && cmd_list->prev != NULL)
		dup2(cmd_list->pip[0], 0);
	else if (cmd_list->input != -1)
	{
		dup2(cmd_list->input, 0);
		close(cmd_list->input);
	}
	if (cmd_list->next != NULL)
	{
		close(cmd_list->next->pip[0]);
		close(cmd_list->next->pip[1]);
	}
	while (cmd_list->prev != NULL)
	{
		close(cmd_list->pip[0]);
		close(cmd_list->pip[1]);
		cmd_list = cmd_list->prev;
	}
}

int	execute_cmd(t_data *data, t_cmd_list cmd_list)
{
	pid_t		pid;
	char		**args;
	char		**env;
	char		*cmd;

	if (cmd_list->cmd[0] == '/' || cmd_list->cmd[0] == '.')
		cmd = ft_strdup(cmd_list->cmd);
	else
		cmd = get_cmd_path(data, cmd_list);
	if (cmd == NULL)
		return (-2);
	ft_printf("cmd: %s\n", cmd);
	pid = fork();
	if (pid == 0)
	{
		if (is_builtin(cmd_list->cmd))
		{
			pipes_work(cmd_list);
			execute_builtin(data, cmd_list);
			exit(0);
		}
		args = args_to_double_pointer(cmd_list->args);
		env = env_to_double_pointer(data->linked_env);
		pipes_work(cmd_list);
		if (execve(cmd, args, env) == -1)
		{
			perror("Error");
			exit(1);
		}
	}
	else if (pid < 0)
		prompt_error("Error: forkgh failed", NULL, data);
	return pid;
}

void	execute(t_data *data)
{
	t_cmd_list	cmd_list;
	// pid_t		pid;

	cmd_list = data->cmd_list;
	while (cmd_list)
	{
		if (cmd_list->parsing_error == 0 && data->parsing_error == 0)
		{
			if (cmd_list->next != NULL)
				pipe(cmd_list->next->pip);
			if (is_builtin(cmd_list->cmd) && cmd_list->next == NULL && cmd_list->prev == NULL)
				execute_builtin(data, cmd_list);
			else
			{
				if (cmd_list->cmd)
					execute_cmd(data, cmd_list);
			}

		}
		cmd_list = cmd_list->next;
	}
	cmd_list = data->cmd_list;
	while (cmd_list)
	{
		if (cmd_list->input != -1 && cmd_list->input != 0)
			close(cmd_list->input);
		if (cmd_list->output != -1 && cmd_list->output != 0)
			close(cmd_list->output);
		cmd_list = cmd_list->next;
	}
	cmd_list = data->cmd_list->next;
	while (cmd_list != NULL)
	{
		close(cmd_list->pip[0]);
		close(cmd_list->pip[1]);
		cmd_list = cmd_list->next;
	}
	while (waitpid(-1, NULL, 0) != -1)
		;

}
