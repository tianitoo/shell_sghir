/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:19:51 by msekhsou          #+#    #+#             */
/*   Updated: 2023/05/15 00:35:20 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], 1);
			ft_printf('\n');
		}
		i++;
	}
}

int	check_slvl(char **env)
{
	int	i;

	i = 0;
	while (env[i++])
	{
		if (!ft_strncmp("SHLVL=", env[i], ft_strlen("SHLVL=")))
			return (1);
	}
	return (0);
}