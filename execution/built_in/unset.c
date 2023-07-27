/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacharai <sacharai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 11:37:08 by hnait             #+#    #+#             */
/*   Updated: 2023/07/21 14:19:36 by sacharai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	move_pointers_back(char **double_pointer_env, int i)
{
	while (double_pointer_env[i])
	{
		double_pointer_env[i] = double_pointer_env[i + 1];
		i++;
	}
	return (1);
}

void	ft_unset(t_data *data, t_cmd *cmd_list)
{
	t_env		*env;
	t_env		*prev;
	t_params	params;
	char		*var_name;

	prev = NULL;
	params = cmd_list->args->next;
	while (params)
	{
		env = data->linked_env;
		var_name = params->parameter;
		while (env)
		{
			if (ft_strcmp(env->key, var_name) == 0)
			{
				if (prev == NULL)
					data->linked_env = env->next;
				else
					prev->next = env->next;
				free(env->key);
				free(env->value);
				free(env);
				break ;
			}
			prev = env;
			env = env->next;
		}
		params = params->next;
	}
}
