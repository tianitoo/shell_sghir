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

void	execute_builtin(t_data *data)
{
	if (ft_strcmp(data->cmd_list->cmd, "echo") == 0)
		ft_echo(data->cmd_list->args);
	else if (ft_strcmp(data->cmd_list->cmd, "cd") == 0)
		ft_cd(data->cmd_list->args, data->env);
	else if (ft_strcmp(data->cmd_list->cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(data->cmd_list->cmd, "export") == 0)
		ft_export(data);
	else if (ft_strcmp(data->cmd_list->cmd, "unset") == 0)
		ft_unset(data);
	else if (ft_strcmp(data->cmd_list->cmd, "env") == 0)
		write_env(data);
	else if (ft_strcmp(data->cmd_list->cmd, "exit") == 0)
		ft_exit(data);
}

char *get_cmd_path_from_paths(char **paths, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0);
		cmd_path = ft_strjoin(tmp, cmd, 1);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(t_data *data, t_cmd_list cmd_list)
{
	char	*cmd;
	char	**env;
	char	*path;
	char	**paths;

	env = data->env;
	path = get_variable(env, "PATH");
	paths = ft_split(path, ':');
	cmd = get_cmd_path_from_paths(paths, cmd_list->cmd);
	if (cmd == NULL)
		cmd = ft_strdup(data->cmd_list->cmd);
	return (cmd);
}

void	execute_cmd(t_data *data, t_cmd_list cmd_list)
{
	pid_t		pid;
	char		**args;
	char		*cmd;

	cmd = get_cmd_path(data, cmd_list);
	args = args_to_double_pointer(cmd_list->args);
	pid = fork();
	if (pid == 0)
	{
		if (cmd_list->next != NULL)
		{
			dup2(cmd_list->pip[1], 1);
			if (cmd_list->input == -1)
				dup2(cmd_list->pip[0], 0);
		}
		if (execve(cmd, args, data->env) == -1)
			ft_putstr_fd("Error: execve failed\n", 2);
	}
	else if (pid < 0)
		ft_putstr_fd("Error: fork failed\n", 2);
}

void	execute(t_data *data)
{
	t_cmd_list	cmd_list;

	cmd_list = data->cmd_list;
	if (is_builtin(data->cmd_list->cmd))
		execute_builtin(data);
	else
	{
		while (cmd_list)
		{
			if (cmd_list->cmd)
				execute_cmd(data, cmd_list);
			cmd_list = cmd_list->next;
		}
		wait(NULL);
	}
}