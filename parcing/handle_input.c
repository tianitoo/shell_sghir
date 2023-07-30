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
	g_exit->g_exit_status = exit_status;
	if (data)
		data->parsing_error = 1;
	if (cmd_list)
		cmd_list->parsing_error = 1;
	ft_printf("%s\n", error);
}

char	*get_commande_line(void)
{
	char		*input;

	input = NULL;
	input = readline("minishell$ ");
	add_garbage(input);
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
		free(tmp->parameter);
		tmp->parameter = NULL;
		free(tmp);
		tmp = NULL;
		tmp = next;
	}
	*params = NULL;
}

void	free_cmd_list(t_cmd_list *cmd_list)
{
	t_cmd_list	tmp;
	t_cmd_list	next;

	if (*cmd_list == NULL)
		return ;
	tmp = *cmd_list;
	while (tmp)
	{
		next = tmp->next;
		// free(tmp->cmd);
		// free_params(&tmp->args);
		// if (tmp->input)
		// 	free(tmp->input);
		// if (tmp->output)
		// 	free(tmp->output);
		// free(tmp->args);
		free(tmp);
		tmp = NULL;
		tmp = next;
	}
	*cmd_list = NULL;
}

void	get_input(t_data *data)
{
	data->commande_line = get_commande_line();
	// add_garbag//e(data->commande_line);
	if (data->commande_line && ft_strlen(data->commande_line) > 0)
		add_history(data->commande_line);
	treat_input(data);
	if (data->params == NULL || data->parsing_error == 1)
		return ;
	data->cmd_list = get_cmd_list(data);
	// ft_printf("commande_line:\n");
	// if (g_exit_status != 0)
	// 	exit(g_exit_status);
	// show_command(data->cmd_list);
	// exec
		// ft_printf("parcing_error: %d\n", data->parsing_error);
	if (data->cmd_list && data->parsing_error == 0)
		execute(data);
	// free_cmd_list(&data->cmd_list);
	free_params(&data->params);
	// free(data->commande_line);
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
