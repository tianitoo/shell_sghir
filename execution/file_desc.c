/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_desc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:48:59 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:59:06 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	close_file_descriptors(t_data *data)
{
	t_cmd_list	cmd_list;

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
}

