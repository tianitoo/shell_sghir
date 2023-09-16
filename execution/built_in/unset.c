/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 11:37:08 by hnait             #+#    #+#             */
/*   Updated: 2023/09/16 01:49:19 by kmouradi         ###   ########.fr       */
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

void	free_env(t_env *env)
{
	if (env)
	{
		free(env->key);
		free(env->value);
		free(env);
	}
}

t_env	*remove_variable(t_data *data, char *var_name)
{
	t_env	*env;
	t_env	*prev;

	env = data->linked_env;
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
		{
			if (prev == NULL)
				data->linked_env = env->next;
			else
				prev->next = env->next;
			break ;
		}
		prev = env;
		env = env->next;
	}
	return (data->linked_env);
}

t_env	*ft_unset(t_data *data, t_cmd *cmd_list)
{
	t_env		*env;
	t_params	params;
	char		*var_name;
	int			i;

	i = 0;
	params = cmd_list->args->next;
	while (params)
	{
		ft_printf("here\n");
		var_name = params->parameter;
		if (var_name[0] == '='
			|| ft_isdigit(var_name[0])
			|| !(ft_isalpha(var_name[0]) || var_name[0] == '_'))
			ft_printf("minishell: unset: `%s': not a valid identifier\n",
				var_name);
		while (var_name[i])
		{
			if (ft_isalnum(var_name[i]) == 0)
			ft_printf("minishell: unset: `%s': not a valid identifier\n",
				var_name);
			i++;
		}
		env = remove_variable(data, var_name);
		params = params->next;
	}
	return (env);
}
