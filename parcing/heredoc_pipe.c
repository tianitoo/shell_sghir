/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:50:34 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 20:01:21 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	*create_heredoc_pipe(t_params next, t_data *data)
{
	int	*pip;
	int	pip_test;

	if (next->is_operator == 1)
		return (prompt_error("minishell: syntax error", NULL, data, 258), NULL);
	pip = (int *)malloc(sizeof(int) * 2);
	if (!pip)
		return (prompt_error("minishell: malloc error", NULL, data, 1), NULL);
	if (add_garbage(data, pip) == NULL)
		return (NULL);
	pip_test = pipe(pip);
	if (pip_test == -1)
		return (prompt_error("minishell: pipe error", NULL, data, 1), NULL);
	return (pip);
}

void	empty_pipe(char *line, int *pip)
{
	if (line == NULL && g_exit->heredoc_ctrlc == 1)
	{
		line = get_next_line(pip[0]);
		while (g_exit->number_of_lines > 1)
		{
			line = get_next_line(pip[0]);
			g_exit->number_of_lines--;
		}
	}
}
