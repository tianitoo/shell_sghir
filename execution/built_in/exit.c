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
#include <sys/_types/_null.h>

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

void	free_enve(t_data *data)
{
	t_env	*env;
	t_env	*last;

	env = data->linked_env;
	while (env)
	{
		if (env->exported == 1)
		{
			if (last)
				last->next = env->next;
			free(env->key);
			free(env->value);
			free(env);
		}
		last = env;
		env = env->next;
	}
}

void	exit_error(char *str, t_data *data)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_enve(data);
	g_exit->g_exit_status = 255;
	exit(g_exit->g_exit_status);
}

int	check_max(char *arg, t_data *data)
{
	if (ft_strlen(arg) < 19)
		return (1);
	else if (ft_strlen(arg) > 19)
		exit_error(arg, data);
	else
	{
		if (ft_strcmp(arg, "9223372036854775807") > 0)
			return (0);
		else
			return (1);
	}
	return (1);
}

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
	if (!ft_only_dig(args[1])
		|| !check_min(args[1], data)
		|| !check_max(args[1], data))
		exit_error(args[1], data);
}

void	*ft_exit(t_params params, t_data *data)
{
	char	**args;

	g_exit->g_exit_status = 0;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	if (!args[1])
	{
		ft_putendl_fd("exit", 1);
		free_enve(data);
		exit(g_exit->g_exit_status);
	}
	check_exit_errors(args, data);
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
