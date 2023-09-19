/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:06 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 04:13:44 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_min(char *arg, t_data *data)
{
	if (ft_strlen(arg) < 20)
		return (1);
	else if (ft_strlen(arg) > 20)
		exit_error(arg, data);
	else
	{
		if (ft_strcmp(arg, "-9223372036854775808") > 0)
			return (0);
		else
			return (1);
	}
	return (1);
}

void	check_exit_errors(char **args, t_data *data)
{
	if (args[1] == NULL)
		return ;
	if (!ft_only_dig(args[1])
		|| !check_min(args[1], data)
		|| !check_max(args[1], data)
		|| args[1][0] == '\0')
		exit_error(args[1], data);
}

void	*ft_exit(t_params params, t_data *data)
{
	char	**args;

	g_exit->g_exit_status = 0;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	check_exit_errors(args, data);
	if (args[1] == NULL)
	{
		ft_putendl_fd("exit", 1);
		free_enve(data);
		exit(g_exit->g_exit_status);
	}
	if (args[2])
	{
		ft_putendl_fd("exit", 2);
		ft_putendl_fd("exit: too many arguments", 2);
		g_exit->g_exit_status = 1;
		return (NULL);
	}
	g_exit->g_exit_status = ft_atoi(args[1]) % 256;
	ft_putendl_fd("exit", 1);
	exit(g_exit->g_exit_status);
	return (NULL);
}
