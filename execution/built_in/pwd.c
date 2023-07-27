/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 17:09:30 by hnait             #+#    #+#             */
/*   Updated: 2023/07/09 18:16:18 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../minishell.h"

// int	g_exit_status;

void	ft_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cwd = getenv("PWD");
	}
	ft_putstr_fd(cwd, 1);
	ft_printf("\n");
	free(cwd);
}