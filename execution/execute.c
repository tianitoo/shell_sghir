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

char	*check_commande(char *path, char *cmd,
	t_cmd_list cmd_list, t_data *data)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(path, "/", 0);
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd, 1);
	if (garbage(cmd_path, data) == NULL)
		return (NULL);
	if (access(cmd_path, F_OK) == 0)
	{
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		else
			return (ft_printf("Error: %s: permission denied", cmd),
				prompt_error("", cmd_list, data, 126), NULL);
	}
	return (NULL);
}

char	*get_cmd_path_from_paths(char **paths, char *cmd,
	t_cmd_list cmd_list, t_data *data)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (is_directory(cmd))
		return (ft_printf("Error: %s: is a directory", cmd),
			prompt_error("", cmd_list, data, 126), NULL);
	if (ft_strlen(cmd) == 0)
		return (NULL);
	while (paths[i])
	{
		cmd_path = check_commande(paths[i], cmd, cmd_list, data);
		if (cmd_path != NULL)
		{
			free(cmd);
			return (cmd_path);
		}
		i++;
	}
	return (ft_printf("Error: %s: command not found 1", cmd),
		prompt_error("", cmd_list, data, 127), NULL);
}

char	*get_cmd_path(t_data *data, t_cmd_list cmd_list)
{
	char	*cmd;
	char	**paths;

	cmd = cmd_list->cmd;
	if (cmd[0] == '\0')
		return (prompt_error("Error: command not found 3", NULL, data, 127),
			NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (cmd);
			else
				return (ft_printf("Error: %s: permission denied 1", cmd),
					prompt_error("", cmd_list, data, 126), NULL);
		}
		else
			return (ft_printf("Error: %s: command not found 4", cmd),
				prompt_error("", cmd_list, data, 127), NULL);
	}
	paths = get_paths(data, cmd_list, cmd);
	if (paths == NULL)
		return (NULL);
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
		exit(g_exit->g_exit_status);
	env = env_to_double_pointer(data->linked_env, data);
	if (env == NULL)
		exit(g_exit->g_exit_status);
	pipes_work(cmd_list);
	if (execve(cmd, args, env) == -1)
	{
		perror("Error");
		exit(1);
	}
	return (0);
}
