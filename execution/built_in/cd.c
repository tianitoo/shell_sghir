/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:31 by hnait             #+#    #+#             */
/*   Updated: 2023/09/11 00:24:50 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*update_env_var(char *var_key, char *value, t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	while (env)
	{
		if (ft_strcmp(env->key, var_key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (env->value == NULL)
				return (NULL);
			break ;
		}
		env = env->next;
	}
	return (env);
}

t_data	*ft_cd(t_params params, t_data *data)
{
	char	*pwd;
	char	**args;
	char	*pwd;
	t_env	*env;
	char	*next_pwd;

	env = data->linked_env;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	if (args[1] == NULL)
	{
		if (chdir(get_variable(env, "HOME")) == -1)
		{
			ft_printf("cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strncmp(args[1], "$HOME", 5) == 0)
	{
		if (chdir(args[1]) == -1)
		{
			ft_printf("cd: HOME not set\n");
			return (NULL);
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
		if (strcmp(args[1], ".") == 0 || strcmp(args[1], "..") == 0 || strcmp(args[1], "./") == 0 || strcmp(args[1], "../") == 0)
		{
			if (chdir(args[1]) == -1)
			{
				ft_printf("cd: %s: No such file or directory\n", args[1]);
				return (NULL);
			}
			else
			{
				if (getcwd(NULL, 0) == NULL)
				{
					pwd = get_env_value("PWD", data);
					if (pwd == NULL)
						return (NULL);
					if (pwd)
					{
						pwd = ft_strjoin(pwd, "/", 0);
						if (pwd == NULL)
							return (NULL);
						pwd = ft_strjoin(pwd, args[1], 0);
						if (pwd == NULL)
							return (NULL);
						if (update_env_var("PWD", pwd, data) == NULL)
							return (NULL);
					}
					else
					{
						ft_printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory");
					}
				}
			}
		}
		else if (chdir(args[1]) == -1)
		{
			ft_printf("cd: %s: No such file or directory\n", args[1]);
			return (NULL);
		}
		else
		{
			pwd = find_pwd(data);
			if (pwd == NULL)
				return (NULL);
			next_pwd = getcwd(NULL, 0);
			if (update_env_var("OLDPWD", pwd, data) == NULL)
				return (NULL);
			if (update_env_var("PWD", next_pwd, data) == NULL)
				return (NULL);
			free(next_pwd);
			next_pwd = NULL;
		}
	}
	return (data);
}
