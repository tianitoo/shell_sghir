/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 14:15:39 by hnait             #+#    #+#             */
/*   Updated: 2023/09/11 00:15:07 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_atoi(const char *str)
{
	long	n;
	long	i;
	int		s;
	long	x;

	i = 0;
	s = 1;
	n = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			s *= -1;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		x = n;
		n = n * 10 + (str[i++] - 48);
		if (n / 10 != x && s == 1)
			return (-1);
		else if (n / 10 != x && s == -1)
			return (0);
	}
	return (n * s);
}

int	ft_only_dig(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_long(char *str)
{
	if (ft_strcmp(str, "9223372036854775807") != 0 && ft_strcmp(str, "-9223372036854775808") != 0)
		return (0);
	return (1);
}

void	*ft_exit(t_params params, t_data *data)
{
	char	**args;

	g_exit->g_exit_status = 0;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	ft_printf("%s\n", args[0]);
	if (!args[1])
		exit(g_exit->g_exit_status);
	if (!args[2] && (!ft_only_dig(args[1]) || !ft_check_long(args[1])))
	{
		ft_printf("minishell: exit: %s: numeric argument required\n", args[1]);
		g_exit->g_exit_status = 255;
		exit(g_exit->g_exit_status);
	}
	else if (!args[2] && ft_only_dig(args[1]))
	{
		g_exit->g_exit_status = ft_atoi(args[1]) % 256;
		exit(g_exit->g_exit_status);
	}
	if (args[2])
	{
		ft_printf("minishell: exit: too many arguments\n");
		g_exit->g_exit_status = 1;
	}
	return (NULL);
}
