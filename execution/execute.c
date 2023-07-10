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
	// else if (ft_strcmp(cmd, "unset") == 0)
	// 	return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	// else if (ft_strcmp(cmd, "exit") == 0)
	// 	return (1);
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
	// else if (ft_strcmp(data->cmd_list->cmd, "unset") == 0)
	// 	ft_unset(data->cmd_list->args);
	else if (ft_strcmp(data->cmd_list->cmd, "env") == 0)
		write_env(data);
	// else if (ft_strcmp(data->cmd_list->cmd, "exit") == 0)
	// 	ft_exit(data->cmd_list->args);
}
void	execute_cmd(t_data *data)
{
	pid_t	pid;
	int		status;
	char	**args;

	args = args_to_double_pointer(data->cmd_list->args);
	pid = fork();
	if (pid == 0)
	{
		if (execve(data->cmd_list->cmd, args, data->env) == -1)
			ft_putstr_fd("Error: execve failed\n", 2);
	}
	else if (pid < 0)
		ft_putstr_fd("Error: fork failed\n", 2);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

void	execute(t_data *data)
{
	if (is_builtin(data->cmd_list->cmd))
		execute_builtin(data);
	else
	{
		if (data->cmd_list->cmd)
			execute_cmd(data);
	}
}