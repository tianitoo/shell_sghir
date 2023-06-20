/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 10:43:18 by hnait             #+#    #+#             */
/*   Updated: 2023/06/09 18:38:31 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_cmd(t_cmd_list cmd_list, t_params param)
{
	// ft_printf("add_cmd param->parameter: %s\n", param->parameter);
	if (is_redirect_operator(param))
	{
		handle_redirection(param, cmd_list);
		if (param->next->next != NULL)
		{
			param->next = param->next->next;
			param->next->prev = param;
		}
		else
			param->next = NULL;
	}
	else if (cmd_list->cmd == NULL)
	{
		if (param->parameter == NULL)
			cmd_list->cmd = NULL;
		else
		{
			cmd_list->cmd = param->parameter;
			add_param(&cmd_list->args, param->parameter);
		}
	}
	else
	{
		add_param(&cmd_list->args, param->parameter);
		param = param->next;
	}
	// ft_printf("add_cmd param 2 3 ->parameter: %s\n", param->parameter);

}

void	add_param(t_params *params, char *param)
{
	t_params	tmp;

	if (*params == NULL)
		*params = new_param(param);
	else
	{
		tmp = *params;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_param(param);
		tmp->next->prev = tmp;
	}
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (prompt_error("malloc error"), NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = NULL;
	cmd->next = NULL;
	return (cmd);
}