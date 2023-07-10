/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 09:55:45 by hnait             #+#    #+#             */
/*   Updated: 2023/07/10 10:12:40 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	write_env(t_data *data)
{
	char	**env;
	int		i;

	env = data->env;
	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_printf(env[i]);
			ft_printf("\n");
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