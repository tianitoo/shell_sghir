/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:33:39 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:34:32 by hnait            ###   ########.fr       */
/*                                                                            */
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

void	*builtin(t_data *data, t_cmd_list cmd_list)
{
	void	*ret;

	ret = 0;
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
		ret = write_env(data);
	else if (ft_strcmp(cmd_list->cmd, "exit") == 0)
		ret = ft_exit(cmd_list->args, data);
	return (ret);
}

void	set_io_builtin(t_cmd_list cmd_list, int *input, int *output)
{
	if (cmd_list->input != -1)
	{
		*input = dup(0);
		dup2(cmd_list->input, 0);
		close(cmd_list->input);
	}
	if (cmd_list->output != -1)
	{
		*output = dup(1);
		dup2(cmd_list->output, 1);
		close(cmd_list->output);
	}
}

void	unset_io_builtin(t_cmd_list cmd_list, int input, int output)
{
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

t_cmd_list	execute_builtin(t_data *data, t_cmd_list cmd_list)
{
	int		input;
	int		output;
	void	*ret;

	set_io_builtin(cmd_list, &input, &output);
	pipes_work(cmd_list);
	ret = builtin(data, cmd_list);
	unset_io_builtin(cmd_list, input, output);
	if (ret == NULL)
		return (NULL);
	return (cmd_list);
}
