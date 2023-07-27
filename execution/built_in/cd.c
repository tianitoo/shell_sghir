/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacharai <sacharai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:31 by hnait             #+#    #+#             */
/*   Updated: 2023/07/27 20:24:06 by sacharai         ###   ########.fr       */
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
			break ;
		}
		env = env->next;
	}
}

void	ft_cd(t_params params, t_data *data)
{
	char	**args;
	char	*path;
	char	*oldpwd;
	char	*pwd;
	t_env	*env;

	env = data->linked_env;
	args = args_to_double_pointer(params);
	oldpwd = getcwd(NULL, 0);
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
		char *ran = malloc(sizeof(char *) *1024);
		int a = chdir(getcwd(ran, 1024));
		path = ft_strdup(args[1]);
		if(strcmp(path, "..") == 0 && a == -1)
		{
			printf("hna khas nprinti getcwd error\n");
		}
		if (chdir(path) == -1)
			ft_printf("cd: %s: No such file or directory\n", path);
		free(path);
	}
	pwd = getcwd(NULL, 0);
	update_env("OLDPWD", oldpwd, env);
	update_env("PWD", pwd, env);
	free(oldpwd);
	free(pwd);
}
