/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 09:55:45 by hnait             #+#    #+#             */
/*   Updated: 2023/07/31 09:04:00 by hnait            ###   ########.fr       */
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
		if (env->value != NULL && ft_strlen(env->value) != 0)
		{
			if (env->hidden == 0)
				ft_printf("%s=%s\n", env->key, env->value);
		}
		env = env->next;
	}
	return (data);
}
