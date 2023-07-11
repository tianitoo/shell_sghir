/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   export.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 15:54:25 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 15:54:27 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../minishell.h"

void	add_var_to_env(t_params env_params, t_env *linked_env)
{
	t_params	tmp;
	t_env		*tmp_env;
	char		*key;
	char		*value;

	tmp = env_params;
	while (tmp)
	{
		  
	}
	key = ft_substr(tmp->parameter, 0, ft_strchr(tmp->parameter, '=') - tmp->parameter);
	value = ft_substr(tmp->parameter, ft_strchr(tmp->parameter, '=') - tmp->parameter + 1,
				ft_strlen(tmp->parameter) - ft_strlen(key));
	
}

void	add_to_env(t_data *data)
{
	t_env	*linked_env;
	char	**env;
	int		i;
	t_params	env_params;
	t_params	tmp;

	i = 0;
	linked_env = data->linked_env;
	env = data->env;
	env_params = double_pointer_to_args(env);
	tmp = env_params;
	// print env_params
	while (tmp)
		tmp = tmp->next;
	add_param(&env_params, data->params->next->parameter);
	add_var_to_env(env_params, linked_env);
	tmp = env_params;
	while (tmp)
		tmp = tmp->next;
	data->env = args_to_double_pointer(env_params);
}

//sort the variables from env before "_" variable


void	ft_export(t_data *data)
{
	int		i;
	t_env	*env;

	env = data->linked_env;
	i = 0;
	if (data->params->next == NULL)
	{
		while (env)
		{
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
			env = env->next;
		}
	}
	else if (ft_strchr(data->params->next->parameter, '='))
		add_to_env(data);
	
}