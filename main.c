/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 17:49:05 by hnait             #+#    #+#             */
/*   Updated: 2023/06/12 14:41:44 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable(char **envp, char *var)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
		{
			path = ft_substr(envp[i], ft_strlen(var) + 1,
					ft_strlen(envp[i]) - ft_strlen(var));
			return (path);
		}
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	(void)argc;
	(void)argv;
	// (void)envp;
	// data->env = (char **) malloc(sizeof(char *) * sizeof(envp));
	data = malloc(sizeof(t_data));
	data->env = envp;
	while (1)
	{
		get_input(data);
	}
}
