/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:27:21 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:27:22 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_list	first_command(t_cmd_list cmd_list,
	t_params handling_param, t_data *data)
{
	if (handling_param->parameter[0] == '|'
		&& handling_param->is_operator == 1)
		return (prompt_error("syntax error near unexpected token `|'",
				NULL, data, 258), NULL);
	cmd_list = new_cmd(data);
	if (cmd_list == NULL)
		return (NULL);
	return (cmd_list);
}

t_cmd_list	next_command(t_cmd_list cmd_list,
	t_params handling_param, t_data *data)
{
	if (handling_param->next == NULL)
		return (prompt_error("syntax error near unexpected token `|'",
				NULL, data, 1), NULL);
	if (handling_param->next->is_operator == 1
		&& handling_param->next->parameter[0] == '|')
		return (prompt_error("syntax error near unexpected token `|'",
				NULL, data, 258), NULL);
	cmd_list->next = new_cmd(data);
	cmd_list->next->prev = cmd_list;
	cmd_list = cmd_list->next;
	cmd_list->next = NULL;
	return (cmd_list);
}

t_cmd_list	add_cmd_list(t_cmd_list cmd_list,
	t_params handling_param, t_data *data)
{
	if (cmd_list == NULL)
	{
		cmd_list = first_command(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
	}
	if (handling_param->parameter[0] == '|' && handling_param->is_operator == 1)
	{
		cmd_list = next_command(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
	}
	else
		if (add_param_to_cmd(cmd_list, handling_param, data) == NULL)
			return (NULL);
	return (cmd_list);
}

t_cmd_list	get_cmd_list(t_data *data)
{
	t_params	handling_param;
	t_cmd_list	cmd_list;
	t_cmd_list	head;

	head = NULL;
	handling_param = data->params;
	cmd_list = NULL;
	while (handling_param)
	{
		cmd_list = add_cmd_list(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
		if (head == NULL)
			head = cmd_list;
		if (handling_param->next == NULL)
			break ;
		handling_param = handling_param->next;
	}
	if (data->parsing_error == 0)
	{
		if (handle_params(&head, data) == 0)
			return (NULL);
	}
	return (head);
}
