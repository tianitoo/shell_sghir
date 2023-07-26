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

void	ft_unset(t_data *data)
{
	t_env	*linked_env;
	t_env	*tmp_linked_env;
	t_env	*tmp1_linked_env;
	char	**double_pointer_env;
	char	*tmp_char;
	char	*key;
	int		i;

	if (data->params->next == NULL)
		return ;
	key = data->params->next->parameter;
	i = 0;
	linked_env = data->linked_env;
	double_pointer_env = data->env;
	tmp_linked_env = linked_env;
	while (tmp_linked_env)
	{
		if (ft_strcmp(tmp_linked_env->key, key) == 0)
		{
			while (linked_env->next != tmp_linked_env)
				linked_env = linked_env->next;
			tmp1_linked_env = tmp_linked_env->next;
			linked_env->next = tmp1_linked_env;
			free(tmp_linked_env->key);
			free(tmp_linked_env->value);
			free(tmp_linked_env);
			break ;
		}
		tmp_linked_env = tmp_linked_env->next;
	}
	while (double_pointer_env[i])
	{
		if (ft_strncmp(find_key(double_pointer_env[i]),
				key, ft_strlen(key)) == 0)
		{
			tmp_char = double_pointer_env[i];
			move_pointers_back(double_pointer_env, i);
			free(tmp_char);
			break ;
		}
		i++;
	}
}
