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

t_cmd_list	execute_builtin(t_data *data, t_cmd_list cmd_list)
{
	int	input;
	int	output;
	void	*ret;

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
		ret = ft_echo(cmd_list->args, data);
	else if (ft_strcmp(cmd_list->cmd, "cd") == 0)
		ret = ft_cd(cmd_list->args, data);
	else if (ft_strcmp(cmd_list->cmd, "pwd") == 0)
		ret = ft_pwd(data);
	else if (ft_strcmp(cmd_list->cmd, "export") == 0)
		ret = ft_export(cmd_list, data);
	else if (ft_strcmp(cmd_list->cmd, "unset") == 0)
		ret = ft_unset(data, cmd_list);
	else if (ft_strcmp(cmd_list->cmd, "env") == 0)
		write_env(data);
	else if (ft_strcmp(cmd_list->cmd, "exit") == 0)
		ret = ft_exit(data);
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
	if (ret == NULL)
		return (NULL);
	return (cmd_list);
}

char *get_cmd_path_from_paths(char **paths, char *cmd, t_cmd_list cmd_list, t_data *data)
{
	int		i;
	char	*cmd_path;
	char	*tmp;
	DIR		*dir;

	(void)data;

	dir = opendir(cmd);
	if (dir != NULL)
	{
		ft_printf("Error: %s: is a directory\n", cmd);
		g_exit->g_exit_status = 126;
		(void)closedir(dir);
		return (NULL);
	}
	i = 0;
	if (cmd[0] == '/' || cmd[0] == '.')
		return (cmd);
	if (paths == NULL)
	{
		ft_printf("Error: %s: no such file or directory\n", cmd);
		g_exit->g_exit_status = 127;
	}
	if (ft_strlen(cmd) == 0)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0);
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd, 1);
		if (!cmd_path)
			return (NULL);
		if (add_garbage(data, cmd_path) == NULL)
			return (NULL);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
			return (cmd_path);
		i++;
	}
	ft_printf("Error: %s: command not found1", cmd);
	prompt_error("", cmd_list, NULL, 127);
	return (NULL);
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
	{	
		paths = ft_split(path, ':');
		if (paths == NULL)
			return (NULL);
	}
	else
	{
		ft_printf("Error: %s: command not found2", cmd_list->cmd);
		prompt_error("", cmd_list, NULL, 127);
		return (NULL);
	}
	cmd = get_cmd_path_from_paths(paths, cmd_list->cmd, cmd_list, data);
	free_ss(paths);
	free(paths);
	if (cmd == NULL)
		return (NULL);
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

	cmd = get_cmd_path(data, cmd_list);
	if (cmd == NULL)
		return (-2);
	pid = fork();
	if (pid == 0)
	{
		if (is_builtin(cmd_list->cmd))
		{
			pipes_work(cmd_list);
			execute_builtin(data, cmd_list);
			exit(0);
		}
		args = args_to_double_pointer(cmd_list->args, data);
		if (args == NULL)
			exit(1);
		env = env_to_double_pointer(data->linked_env, data);
		if (env == NULL)
			exit(1);
		pipes_work(cmd_list);
		if (execve(cmd, args, env) == -1)
		{
			perror("Error");
			exit(1);
		}
	}
	else if (pid < 0)
	{
		prompt_error("Error: fork failed", NULL, data, 1);
		g_exit->g_exit_status = 1;
	}
	return pid;
}

void	execute(t_data *data)
{
	t_cmd_list	cmd_list;
	pid_t		pid;

	pid = 0;
	cmd_list = data->cmd_list;
	while (cmd_list)
	{
		if (cmd_list->parsing_error == 0 && data->parsing_error == 0)
		{
			if (cmd_list->next != NULL)
				pipe(cmd_list->next->pip);
			if (is_builtin(cmd_list->cmd) && cmd_list->next == NULL && cmd_list->prev == NULL)
			{
				if (execute_builtin(data, cmd_list) == NULL)
					break ; // same check
			}
			else
			{
				if (cmd_list->cmd)
				{
					pid = execute_cmd(data, cmd_list);
					if (pid == -2)
						break ;// check when pid == -2
				}
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
	waitpid(pid, &g_exit->g_exit_status, 0);
	while (waitpid(-1, NULL, 0) != -1)
		;
}
