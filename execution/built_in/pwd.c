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

void	ft_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_printf("error: could not get current directory\n");
		return ;
	}
	ft_putstr_fd(cwd, 1);
	ft_printf("\n");
	free(cwd);
}