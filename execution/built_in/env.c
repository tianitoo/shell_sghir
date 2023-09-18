/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:10 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:58:49 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*write_env(t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (env == NULL)
		return (NULL);
	while (env)
	{
		if (env->hidden == 0)
		{
			if (env->value != NULL && ft_strlen(env->value) > 0)
				ft_printf("%s=%s\n", env->key, env->value);
			else if (env->show_value == 1)
				ft_printf("%s=\n", env->key);
		}
		env = env->next;
	}
	return (data);
}

int	cwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (free(cwd),
			prompt_error("cd: error retrieving current directory",
				NULL, data, 1), 0);
	free(cwd);
	return (1);
}
