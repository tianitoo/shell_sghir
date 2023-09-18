/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   redirect.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/09/17 22:31:12 by kmouradi		  #+#	#+#			 */
/*   Updated: 2023/09/17 22:31:25 by kmouradi		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirect_operator(t_params param)
{
	if (ft_strncmp(param->parameter, ">", 2) == 0
		|| ft_strncmp(param->parameter, "<", 2) == 0
		|| ft_strncmp(param->parameter, ">>", 3) == 0
		|| ft_strncmp(param->parameter, "<<", 3) == 0)
		return (1);
	return (0);
}

int	handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data)
{
	if (params->parameter[0] == '<' && ft_strlen(params->parameter) == 1)
		add_input(params, cmd_list, data);
	else if (params->parameter[0] == '>' && ft_strlen(params->parameter) == 1)
		add_output(params, cmd_list, data);
	else if (params->parameter[0] == '<' && params->parameter[1] == '<')
	{
		if (handle_heredoc(params, cmd_list, data) == 0)
			return (0);
	}
	else if (params->parameter[0] == '>' && params->parameter[1] == '>')
		if (handle_append(params, cmd_list, data) == NULL)
			return (0);
	return (1);
}

t_params	handle_append(t_params params, t_cmd_list cmd_list, t_data *data)
{
	int			fd;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
		{
			ft_printf("minishell: syntax error near unexpected token `%s'",
				params->next->parameter);
			return (prompt_error("", NULL, data, 258), NULL);
		}
		fd = open_file(params->next->parameter, cmd_list, 1);
		if (fd == -1)
			return (NULL);
		if (cmd_list->output != -1)
			close(cmd_list->output);
		cmd_list->output = fd;
		skip_riderection(params, cmd_list);
	}
	else
		return (prompt_error("minishell: syntax error", NULL, data, 258), NULL);
	return (params);
}

t_params	add_input(t_params params, t_cmd_list cmd_list, t_data *data)
{
	int			fd;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
			return (prompt_error("minishell: syntax error",
					NULL, data, 258), NULL);
		fd = open_file(params->next->parameter, cmd_list, 0);
		if (cmd_list->input != -1)
			close(cmd_list->input);
		cmd_list->input = fd;
		skip_riderection(params, cmd_list);
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
	return (params);
}

t_params	add_output(t_params params, t_cmd_list cmd_list, t_data *data)
{
	int			fd;

	if (params->next != NULL)
	{
		if (params->next->is_operator == 1)
			return (prompt_error("minishell: syntax error",
					NULL, data, 258), NULL);
		fd = open_file(params->next->parameter, cmd_list, 2);
		if (fd == -1)
			return (NULL);
		if (cmd_list->output != -1)
			close(cmd_list->output);
		cmd_list->output = fd;
		skip_riderection(params, cmd_list);
	}
	else
		prompt_error("minishell: syntax error", NULL, data, 258);
	return (params);
}
