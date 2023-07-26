/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   fill_cmd_list.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/02 10:43:18 by hnait			 #+#	#+#			 */
/*   Updated: 2023/06/09 18:38:31 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

void	add_cmd(t_cmd_list cmd_list, t_params param)
{
	t_params	last_param;

	if (cmd_list->cmd == NULL)
	{
		if (param->parameter == NULL)
			cmd_list->cmd = NULL;
		else
		{
			cmd_list->cmd = param->parameter;
			last_param = get_last_param(cmd_list->args);
			add_param(&cmd_list->args, param->parameter);
			get_last_param(cmd_list->args)->prev = last_param;
		}
	}
	else
	{
		add_param(&cmd_list->args, param->parameter);
		param = param->next;
	}
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

t_cmd	*new_cmd(t_data *data)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (prompt_error("malloc error", NULL, data), NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	cmd->parsing_error = 0;
	cmd->input = -1;
	cmd->output = -1;
	cmd->append = -1;
	cmd->pip = (int *)malloc(sizeof(int) * 2);
	if (cmd->pip == NULL)
		return (prompt_error("malloc error", NULL, data), NULL);
	cmd->next = NULL;
	return (cmd);
}