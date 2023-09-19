/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:24:09 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 20:40:51 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_params	new_param(char *param, t_data *data)
{
	t_params	new;

	new = malloc(sizeof(t_param)); 
	if (new == NULL)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);
	}
	new->parameter = param;
	new->next = NULL;
	new->prev = NULL;
	new->is_operator = 0;
	new->in_double_quote = -1;
	new->in_quote = -1;
	return (new);
}

int	handle_param(t_cmd_list tmp, t_data *data)
{
	t_params	params;

	params = tmp->args;
	while (params && tmp->parsing_error == 0 && data->parsing_error == 0)
	{
		if ((params->parameter[0] == '>' || params->parameter[0] == '<')
			&& params->is_operator == 1)
			if (handle_redirection(params, tmp, data) == 0)
				return (0);
		params = params->next;
	}
	if (tmp->cmd == NULL)
	{
		if (tmp->args && tmp->args->parameter)
		{
			tmp->cmd = ft_strdup(tmp->args->parameter);
			if (add_garbage(data, tmp->cmd) == NULL)
				return (0);
		}
	}
	return (1);
}

int	handle_params(t_cmd_list *cmd_list, t_data *data)
{
	t_cmd_list	tmp;

	tmp = *cmd_list;
	while (tmp)
	{
		if (handle_param(tmp, data) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

t_params	clone_t_params(t_params params, t_data *data)
{
	t_params	new;

	new = malloc(sizeof(t_param));
	if (new == NULL)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);
	}
	if (add_garbage(data, new) == NULL)
		return (NULL);
	new->parameter = ft_strdup(params->parameter);
	if (new->parameter == NULL)
		return (NULL);
	if (add_garbage(data, new->parameter) == NULL)
		return (NULL);
	new->is_operator = params->is_operator;
	new->in_double_quote = params->in_double_quote;
	new->in_quote = params->in_quote;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_params	add_param_to_cmd(t_cmd_list cmd_list, t_params param, t_data *data)
{
	t_params	last_param;

	if (cmd_list->args == NULL)
	{
		cmd_list->args = clone_t_params(param, data);
		if (cmd_list->args == NULL)
			return (NULL);
	}
	else
	{
		last_param = get_last_param(cmd_list->args);
		last_param->next = clone_t_params(param, data);
		if (last_param->next == NULL)
			return (NULL);
		last_param->next->prev = last_param;
	}
	return (cmd_list->args);
}
