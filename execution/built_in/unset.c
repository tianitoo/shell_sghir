/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   unset.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/12 11:37:08 by hnait			 #+#	#+#			 */
/*   Updated: 2023/09/11 00:23:59 by kmouradi		 ###   ########.fr	   */
/*																			*/
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

int	delete_variable(t_data *data, char *var_name, t_env *env, t_env *prev)
{
	t_env	*free_me;

	if (ft_strcmp(env->key, var_name) == 0 && env->unsetable == 0)
	{
		free_me = env;
		if (prev == NULL)
			data->linked_env = env->next;
		else
			prev->next = env->next;
		free_env(free_me);
		return (1);
	}
	return (0);
}

t_env	*remove_variable(t_data *data, char *var_name)
{
	t_env	*env;
	t_env	*prev;
	int		i;

	i = 0;
	prev = NULL;
	env = data->linked_env;
	if (!ft_isalpha(var_name[0]) && var_name[0] != '_')
		return (ft_printf("not a valid identifier\n"), env);
	while (var_name[i])
	{
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
			return (ft_printf("not a valid identifier\n"), env);
		i++;
	}
	while (env)
	{
		if (delete_variable(data, var_name, env, prev))
			break ;
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

	params = cmd_list->args->next;
	while (params)
	{
		var_name = params->parameter;
		env = remove_variable(data, var_name);
		params = params->next;
	}
	return (env);
}
