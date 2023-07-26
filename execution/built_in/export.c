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

void	add_var_to_env(char **env, t_env *linked_env)
{
	int		i;
	t_env	*tmp;
	char	**new_env;

	i = 0;
	tmp = linked_env;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup(tmp->key);
	new_env[i + 1] = NULL;
	free_ss(env);	
	env = new_env;
}



void	update_param(t_params env_params, char *key, char *new_param)
{
	t_params	tmp;

	tmp = env_params;
	while (tmp)
	{
		if (ft_strncmp(find_key(tmp->parameter), key, ft_strlen(key)) == 0)
		{
			tmp->parameter = new_param;
			break ;
		}
		tmp = tmp->next;
	}
}

int	key_exists(t_params env_params, char *key)
{
	t_params	tmp;

	tmp = env_params;
	while (tmp)
	{
		if (ft_strncmp(find_key(tmp->parameter), key, ft_strlen(key)) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	 add_to_env(t_data *data)
{
	// t_env	*linked_env;
	char	**env;
	char	*key;
	t_params	env_params;
	// t_params	tmp;

	// linked_env = data->linked_env;
	env = data->env;
	key = find_key(data->params->next->parameter);
	// exit(1);
	env_params = double_pointer_to_args(env);
	if (!key_exists(env_params, key))
	{
		add_param(&env_params, data->params->next->parameter);
		free_ss(env);
	}
	else
	{
		update_param(env_params, key, data->params->next->parameter);
		free_ss(env);
	}
	data->env = args_to_double_pointer(env_params);
	// print data->env
	ft_printf("data->env:\n");
	int i = 0;
	while (data->env[i])
	{
		ft_printf("%s\n", data->env[i]);
		i++;
	}
}

void	ft_export(t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (data->params->next == NULL)
	{
		printf("=====================================\n");
		while (env)
		{
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
			env = env->next;
		}
		printf("=====================================\n");
	}
	else if (ft_strchr(data->params->next->parameter, '='))
		add_to_env(data);
}
