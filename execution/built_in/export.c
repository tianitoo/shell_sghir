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

void	add_to_env(t_data *data)
{
	char	**env;
	int		i;
	t_params	env_params;
	t_params	tmp;

	i = 0;
	env = data->env;
	env_params = double_pointer_to_args(env);
	tmp = env_params;
	// print env_params
	while (tmp)
	{
		ft_printf("tmp->parameter = %s\n", tmp->parameter);
		tmp = tmp->next;
	}
	add_param(&env_params, data->params->next->parameter);
	tmp = env_params;
	while (tmp)
	{
		ft_printf("tmp->parameter = %s\n", tmp->parameter);
		tmp = tmp->next;
	}
	data->env = args_to_double_pointer(env_params);
	ft_printf("here\n");
}

void	ft_export(t_data *data)
{
	int		i;
	char	**env;

	env = data->env;
	i = 0;
	if (data->params->next == NULL)
	{
		while (env[i])
		{
			ft_printf("declare -x %s\n", env[i]);
			i++;
		}
	}
	else if (ft_strchr(data->params->next->parameter, '='))
		add_to_env(data);
	
}