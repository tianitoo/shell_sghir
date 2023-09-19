/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:24:32 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 01:54:57 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit	*g_exit = NULL;

t_data	*set_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (prompt_error("malloc error", NULL, NULL, 1), NULL);
	data->params = NULL;
	data->linked_env = get_linked_env(envp, data);
	if (!data->linked_env)
		return (NULL);
	add_hidden_env(data->linked_env, "PATH",
		ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."), data);
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	(void)argc;
	(void)argv;
	if (set_g_exit())
		return (1);
	rl_catch_signals = 0;
	data = set_data(envp);
	if (!data)
		return (1);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_exit->in_exec_mode = 0;
		data->parsing_error = 0;
		get_input(data);
		free_garbage();
	}
	return (0);
}
