/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 10:39:31 by hnait             #+#    #+#             */
/*   Updated: 2023/06/02 13:49:36 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redirect_operator(t_params param)
{
	if (ft_strncmp(param->parameter, ">", 2) == 0
		|| ft_strncmp(param->parameter, "<", 2) == 0
		|| ft_strncmp(param->parameter, ">>", 3) == 0
		|| ft_strncmp(param->parameter, "<<", 3) == 0)
		return (1);
	return (0);
}

void handle_redirection(t_params params, t_cmd_list cmd_list)
{
	// t_params	prev;
	// t_params	tmp;

	if (params->parameter[0] == '<' && ft_strlen(params->parameter) == 1)
		add_input(params, cmd_list);
	else if (params->parameter[0] == '>' && ft_strlen(params->parameter) == 1)
		add_output(params, cmd_list);
	// else if (params->parameter[0] == '<' && params->parameter[1] == '<')
	// 	handle_heredoc(params, cmd_list);
	// // else if (params->parameter[0] == '>' && params->parameter[1] == '>')
	// // 	handle_append(params, cmd_list);
	// // else if (params->parameter[0] == '<' && params->parameter[1] == '>'
	// // 	|| params->parameter[0] == '>' && params->parameter[1] == '<')
	// // 	prompt_error("minishell: syntax error near unexpected token `newline'");
}

void	add_input(t_params params, t_cmd_list cmd_list)
{
	t_params	prev;

	if (params->next != NULL)
	{
		cmd_list->input = params->next->parameter;
		prev = params->prev;
		if (prev == NULL)
		{
			if (params->next->next != NULL)
				params = params->next->next;
			else
				params = NULL;
		}
		else
		{
			if (params->next->next != NULL)
				prev->next = params->next->next;
			else
				prev->next = NULL;
		}
	}
	else
		prompt_error("minishell: syntax error near unexpected token `newline'");
}

void	add_output(t_params params, t_cmd_list cmd_list)
{
	t_params	prev;

	if (params->next != NULL)
	{
		cmd_list->output = params->next->parameter;
		prev = params->prev;
		if (prev == NULL)
		{
			if (params->next->next != NULL)
				cmd_list->args = params->next->next;
			else
				cmd_list->args = NULL;
		}
		else
		{
			if (params->next->next != NULL)
				prev->next = params->next->next;
			else
				prev->next = NULL;
		}
	}
}
