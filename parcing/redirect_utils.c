/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:18 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 03:59:51 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *file, t_cmd_list cmd_list, int redirect_type)
{
	int		fd;
	DIR		*dir;

	fd = -1;
	dir = opendir(file);
	if (dir != NULL)
		return (closedir(dir), ft_printf("%s: is a directory",
				file), prompt_error(" ", cmd_list, NULL, 126), -1);
	if (redirect_type == 0)
		fd = open(file, O_RDONLY);
	else if (redirect_type == 1)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redirect_type == 2)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (prompt_error("minishell: no such file or directory  d",
				cmd_list, NULL, 1), -1);
	return (fd);
}

void	skip_riderection(t_params params, t_cmd_list cmd_list)
{
	if (params->prev)
		params->prev->next = params->next->next;
	if (params->prev == NULL)
		cmd_list->args = params->next->next;
	if (params->next->next)
		params->next->next->prev = params->prev;
}
