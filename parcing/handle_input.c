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

void	prompt_error(char *error)
{
	ft_printf("%s\n", error);
}

char	*get_commande_line(void)
{
	char		*input;

	input = readline("minishell$ ");
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
		// free(tmp);
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
		if (tmp->input)
			free(tmp->input);
		if (tmp->output)
			free(tmp->output);
		// free(tmp->args);
		free(tmp);
		tmp = next;
	}
	*cmd_list = NULL;
}

void	get_input(t_data *data)
{
	data->commande_line = ft_strdup(get_commande_line());
	add_history(data->commande_line);
	treat_input(data);
	if (data->params == NULL)// || g_exit_status != 0)
		return ;
	data->cmd_list = get_cmd_list(data->params);
	// ft_printf("commande_line:\n");
	// if (g_exit_status != 0)
	// 	exit(g_exit_status);
	// show_command(data->cmd_list);
	// exec
	if (data->cmd_list)
			execute(data);
	free_cmd_list(&data->cmd_list);
	free_params(&data->params);
	free(data->commande_line);
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
		ft_printf("cmd: %s\n", tmp->cmd);
		tmp_params = tmp->args;
		ft_printf("args:\n");
		while (tmp_params)
		{
			ft_printf("%s\n", tmp_params->parameter);
			tmp_params = tmp_params->next;
		}
		ft_printf("\n");
		ft_printf("in: %s\n", tmp->input);
		ft_printf("out: %s\n", tmp->output);
		ft_printf("append: %s\n", tmp->append);
		ft_printf("\n");
		tmp = tmp->next;
		ft_printf("=============================================\n");
	}
}
