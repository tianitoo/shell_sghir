/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   args_double_pointer.c							  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 14:29:14 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:39:30 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

char	**args_to_double_pointer(t_params params)
{
	int	 i;
	int		args_count;
	char	**args;
	t_params	tmp;

	i = 0;
	tmp = params;
	args_count = 0;
	while (tmp)
	{
		args_count++;
		tmp = tmp->next;
	}
	args = (char **)ft_calloc(sizeof(char *), (args_count + 1));
	add_garbage(args);
	while (params)
	{
		args[i] = ft_strdup(params->parameter);
		add_garbage(args[i]);
		params = params->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

char	**env_to_double_pointer(t_env *env)
{
	int	 i;
	int		env_count;
	char	**envp;
	t_env	*tmp;

	i = 0;
	tmp = env;
	env_count = 0;
	while (tmp)
	{
		env_count++;
		tmp = tmp->next;
	}
	envp = (char **)ft_calloc(sizeof(char *), (env_count + 1));
	while (env)
	{
		envp[i] = ft_strjoin(env->key, "=", 0);
		add_garbage(envp[i]);
		envp[i] = ft_strjoin(envp[i], env->value, 0);
		add_garbage(envp[i]);
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}