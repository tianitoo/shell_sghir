/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:31 by hnait             #+#    #+#             */
/*   Updated: 2023/07/31 22:37:06 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

void	update_env(char *var_name, char *var_value, t_env *env)
{
	char	*tmp;

	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
		{
			tmp = env->value;
			env->value = ft_strdup(var_value);
			free(tmp);
			tmp = NULL;
			break ;
		}
		env = env->next;
	}
}

void	update_env_var(char *var_key, char *value, t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	while (env)
	{
		if (ft_strcmp(env->key, var_key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			break ;
		}
		env = env->next;
	}
}

void	ft_cd(t_params params, t_data *data)
{
	char	**args;
	// char	*path;
	// char	*oldpwd;
	char	*pwd;
	t_env	*env;
	char	*next_pwd;
	// DIR		*dir;

	env = data->linked_env;
	args = args_to_double_pointer(params);
	// add_garbage(oldpwd);
	if (args[1] == NULL)
	{
		if (chdir(get_variable(env, "HOME")) == -1)
		{
			ft_printf("cd: HOME not set\n");
			return ;
		}
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		if (chdir(get_variable(env, "OLDPWD")) == -1)
			ft_printf("cd: OLDPWD not set\n");
		else
			ft_printf("%s\n", get_variable(env, "OLDPWD"));
	}
	else
	{
		if (chdir(args[1]) == -1)
		{
			ft_printf("cd: %s: No such file or directory\n", args[1]);
			return ;
		}
		else
		{
			next_pwd = getcwd(NULL, 0);
			update_env_var("OLDPWD", get_variable(env, "PWD"), data);
			update_env_var("PWD", next_pwd, data);
			free(next_pwd);
			next_pwd = NULL;
		}
	}
}
