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

char	**add_var_to_env(char **env, t_env *linked_env)
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
	return (new_env);
}

char	*find_key(char *str)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

void	update_param(t_env *env_params, char *key, char *new_param)
{
	t_env	*tmp;

	tmp = env_params;
	while (tmp)
	{
		if (ft_strcmp(find_key(key), tmp->key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(new_param);
		}
		tmp = tmp->next;
	}
}

int	key_exists(t_env *env_params, char *key)
{
	t_env	*tmp;

	tmp = env_params;
	while (tmp)
	{
		if (ft_strcmp(find_key(key), tmp->key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

char	*get_value(char *variable)
{
	int		i;
	char	*value;

	i = 0;
	while (variable[i] != '=' && variable[i])
		i++;
	if (variable[i] == '\0')
		return (NULL);
	value = ft_substr(variable, i + 1, ft_strlen(variable));
	return (value);
}

void	 add_to_env(t_data *data)
{
	// t_env	*linked_env;
	char	*key;
	char	*value;
	t_env	*linked_env;

	linked_env = data->linked_env;
	key = find_key(data->params->next->parameter);
	if (key_exists(data->linked_env, key))
		update_param(data->linked_env, key, data->params->next->parameter);
	else
	{
		value = get_value(data->params->next->parameter);
		while (linked_env->next)
			linked_env = linked_env->next;
		linked_env->next = new_env(key, value);
	}

}

void	ft_export(t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (data->params->next == NULL)
	{
		while (env)
		{
			ft_printf("declare -x %s", env->key);
			if (env->value)
				ft_printf("=\"%s\"\n", env->value);
			else
				ft_printf("\n");
			env = env->next;
		}
	}
	else if (data->params->next != NULL)
		add_to_env(data);
}
