/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacharai <sacharai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:31 by hnait             #+#    #+#             */
/*   Updated: 2023/07/27 19:14:10 by sacharai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

void	update_env(char *var_name, char *var_value, char **env)
{
	int		i;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
		{
			tmp = ft_strjoin(var_name, "=", 0);
			// free(env[i]);
			env[i] = ft_strjoin(tmp, var_value, 0);
			free(tmp);
			break ;
		}
		i++;
	}
}

void	ft_cd(t_params params, char **env)
{
	char	**args;
	char	*path;
	char	*oldpwd;
	char	*pwd;

	args = args_to_double_pointer(params);
	oldpwd = getcwd(NULL, 0);
	if (args[1] == NULL)
	{
		if (chdir(get_variable(env, "HOME")) == -1
			|| chdir(get_variable(env, "home")) == -1)
			ft_printf("cd: HOME not set\n");
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
