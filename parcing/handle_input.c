/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   handle_input.c									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/05/11 10:47:00 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:53:00 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_error(char *error, t_cmd_list cmd_list, t_data *data, int exit_status)
{
	if (g_exit)
		g_exit->g_exit_status = exit_status;
	if (data)
		data->parsing_error = 1;
	else if (cmd_list)
		cmd_list->parsing_error = 1;
	if (data && data->commande_line && ft_strlen(data->commande_line) > 0)
		add_history(data->commande_line);
	ft_printf("%s\n", error);
}

char	*get_commande_line(t_data *data)
{
	char	*input;

	(void)data;
	input = NULL;
	input = readline("minishell$ ");
	if (add_garbage(data, input) == NULL)
		return (NULL);
	if (input == NULL)
	{
		ft_printf("exit\n");
		exit(0);
	}
	return (input);
}

void	free_params(t_params *params)
{
	t_params	tmp;
	t_params	next;

	if (*params == NULL)
		return ;
	tmp = *params;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->parameter)
			free(tmp->parameter);
		tmp->parameter = NULL;
		if (tmp)
			free(tmp);
		tmp = NULL;
		tmp = next;
	}
	*params = NULL;
}

void	get_input(t_data *data)
{
	data->commande_line = get_commande_line(data);
	g_exit->in_execMode = 1;
	if (data->commande_line == NULL)
		return ;
	if (!treat_input(data))
	{
		free_params(&data->params);
		return ;
	}
	if (data->params == NULL || data->parsing_error == 1)
	{
		free_params(&data->params);
		return ;
	}
	data->cmd_list = get_cmd_list(data);
	if (data->cmd_list == NULL || data->parsing_error == 1)
	{
		free_params(&data->params);
		return ;
	}
	if (data->cmd_list && data->parsing_error == 0)
		execute(data);
	if (data->commande_line && ft_strlen(data->commande_line) > 0)
		add_history(data->commande_line);
	free_params(&data->params);
}

void	show_command(t_cmd_list cmd_list)
{
	t_cmd_list	tmp;
	t_params	tmp_params;

	if (cmd_list == NULL)
		return ;
	tmp = cmd_list;
	while (tmp)
	{
		ft_printf("=============================================\n");
		ft_printf("cmd: |%s|\n", tmp->cmd);
		tmp_params = tmp->args;
		ft_printf("args:\n");
		while (tmp_params)
		{
			ft_printf(" arg: |%s| len: %d is_operator: %d\n", tmp_params->parameter, ft_strlen(tmp_params->parameter), tmp_params->is_operator);
			tmp_params = tmp_params->next;
		}
		ft_printf("\n");
		ft_printf("in: %d\n", tmp->input);
		ft_printf("out: %d\n", tmp->output);
		ft_printf("\n");
		tmp = tmp->next;
		ft_printf("=============================================\n");
	}
}
