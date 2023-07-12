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
	char		*key;
	char		*value;

	tmp = env_params;
	while (tmp->next)
		tmp = tmp->next;
	key = ft_substr(tmp->parameter, 0, ft_strchr(tmp->parameter, '=') - tmp->parameter);
	value = ft_substr(tmp->parameter, ft_strchr(tmp->parameter, '=') - tmp->parameter + 1,
				ft_strlen(tmp->parameter) - ft_strlen(key));
	add_env(linked_env, key, value);
}

char	*find_key(char *str)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

void	update_param(t_params env_params, char *key, char *new_param)
{
	t_params	tmp;

	tmp = env_params;
	while (tmp->next)
	{
		if (ft_strncmp(find_key(tmp->parameter), key, ft_strlen(key)) == 0)
		{
			tmp->parameter = new_param;
			break ;
		}
		tmp = tmp->next;
	}
}

void	update_linked_env(t_env *linked_env, char *key, char *new_param)
{
	t_env	*tmp;
	char	*value;

	tmp = linked_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			value = ft_substr(new_param, ft_strchr(new_param, '=') - new_param + 1,
				ft_strlen(new_param) - ft_strlen(key));
			tmp->value = value;
			break ;
		}
		tmp = tmp->next;
	}
}

int	key_exists(t_params env_params, char *key)
{
	t_params	tmp;

	tmp = env_params;
	while (tmp->next)
	{
		if (ft_strncmp(find_key(tmp->parameter), key, ft_strlen(key)) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	 add_to_env(t_data *data)
{
	t_env	*linked_env;
	char	**env;
	char	*key;
	t_params	env_params;
	// t_params	tmp;

	linked_env = data->linked_env;
	env = data->env;
	
	key = find_key(data->params->next->parameter);
	env_params = double_pointer_to_args(env);
		ft_printf("key exists\n");
	if (!key_exists(env_params, key))
	{
		add_param(&env_params, data->params->next->parameter);
		add_var_to_env(env_params, linked_env);
	} else {
		update_param(env_params, key, data->params->next->parameter);
		update_linked_env(linked_env, key, data->params->next->parameter);
	}
	data->env = args_to_double_pointer(env_params);
}

void	ft_export(t_data *data)
{
	// int		i;
	t_env	*env;

	env = data->linked_env; 
	// i = 0;
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