/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacharai <sacharai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 09:55:45 by hnait             #+#    #+#             */
/*   Updated: 2023/07/25 17:49:38 by sacharai         ###   ########.fr       */
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
			ft_printf("%s\n", env[i]);
		}
		i++;
	}
	printf("here\n");
}
