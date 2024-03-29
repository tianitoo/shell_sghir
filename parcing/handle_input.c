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

void	prompt_error(char *error, t_cmd_list cmd_list,
	t_data *data, int exit_status)
{
	if (g_exit)
		g_exit->g_exit_status = exit_status;
	if (data)
		data->parsing_error = 1;
	else if (cmd_list)
		cmd_list->parsing_error = 1;
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

int	get_input(t_data *data)
{
	data->commande_line = get_commande_line(data);
	if (data->commande_line == NULL)
		return (0);
	data->original_commande_line = ft_strdup(data->commande_line);
	if (garbage(data->original_commande_line, data) == NULL)
		return (0);
	g_exit->in_exec_mode = 1;
	if (!treat_input(data))
		return (free_params(&data->params), 0);
	if (*data->original_commande_line)
		add_history(data->original_commande_line);
	if (data->params == NULL || data->parsing_error == 1)
		return (free_params(&data->params), 0);
	data->cmd_list = get_cmd_list(data);
	if (data->cmd_list == NULL || data->parsing_error == 1)
		return (free_params(&data->params), 0);
	if (data->cmd_list && data->parsing_error == 0)
		execute(data);
	return (free_params(&data->params), 1);
}
