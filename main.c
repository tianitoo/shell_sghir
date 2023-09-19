/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:24:32 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 20:41:53 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit	*g_exit = NULL;

char	**get_paths(t_data *data, t_cmd_list cmd_list, char *cmd)
{
	char	**paths;
	char	*path;
	t_env	*env;

	env = data->linked_env;
	path = get_variable(env, "PATH");
	if (path != NULL)
	{
		paths = ft_split(path, ':');
		if (paths == NULL)
			return (prompt_error("Error: malloc failed", NULL, data, 1), NULL);
	}
	else
	{
		ft_printf("Error: %s: command not found", cmd);
		prompt_error("", cmd_list, data, 127);
		return (NULL);
	}
	return (paths);
}

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
