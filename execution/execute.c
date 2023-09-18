/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   execute.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/09/15 17:26:44 by kmouradi		  #+#	#+#			 */
/*   Updated: 2023/09/15 17:35:25 by kmouradi		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(char *cmd)
{
	DIR	*dir;

	dir = opendir(cmd);
	if (dir != NULL)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

char	*get_cmd_path_from_paths(char **paths, char *cmd,
	t_cmd_list cmd_list, t_data *data)
{
	int		i;
	char	*cmd_path;
	char	*tmp;

	i = 0;
	if (is_directory(cmd))
		return (ft_printf("Error: %s: is a directory", cmd),
			prompt_error("", cmd_list, data, 126), NULL);
	if (ft_strlen(cmd) == 0)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0);
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd, 1);
		if (garbage(cmd_path, data) == NULL)
			return (NULL);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
			return (cmd_path);
		i++;
	}
	return (ft_printf("Error: %s: command not found1", cmd),
		prompt_error("", cmd_list, data, 127), NULL);
}

char	**get_paths(t_data *data, t_cmd_list cmd_list, char *cmd)
{
	char	**paths;
	char	*path;
	t_env	*env;

	env = data->linked_env;

	path = get_variable(env, "PATH");
	if (path != NULL)
	{
		paths = ft_split(path, ':');
		if (paths == NULL)
			return (prompt_error("Error: malloc failed", NULL, data, 1), NULL);
	}
	else
	{
		ft_printf("Error: %s: command not found", cmd);
		prompt_error("", cmd_list, data, 127);
		return (NULL);
	}
	return (paths);
}

char	*get_cmd_path(t_data *data, t_cmd_list cmd_list)
{
	char	*cmd;
	char	**paths;
	cmd = cmd_list->cmd;
	if (cmd[0] == '/' || cmd[0] == '.')
		return (cmd);
	paths = get_paths(data, cmd_list, cmd);
	cmd = get_cmd_path_from_paths(paths, cmd, cmd_list, data);
	free_ss(paths);
	free(paths);
	if (cmd == NULL)
		return (NULL);
	return (cmd);
}

int	set_up_execve(t_cmd_list cmd_list, t_data *data)
{
	char	**args;
	char	**env;
	char	*cmd;

	cmd = get_cmd_path(data, cmd_list);
	if (cmd == NULL)
		return (-2);
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
	return (0);
}
