/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 14:15:39 by hnait             #+#    #+#             */
/*   Updated: 2023/09/02 15:50:01 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*ft_exit(t_params params, t_data *data)
{
	char **args;
	g_exit->g_exit_status = 0;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	ft_printf("%s\n", args[0]);
	if (!args[1])
		exit(g_exit->g_exit_status);
	if (args[1])
	{
		g_exit->g_exit_status = ft_atoi(args[1]) % 256;
		exit(g_exit->g_exit_status);
	}
	return (NULL);
}




//check if args[1] exist
//