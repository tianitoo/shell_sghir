/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:31 by hnait             #+#    #+#             */
/*   Updated: 2023/09/15 17:38:32 by kmouradi         ###   ########.fr       */
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

int	cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (0);
	free(cwd);
	return (1);
}

char	*current_parrent_dir(char **args, t_data *data)
{
	char	*pwd;

	pwd = 0;
	if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory\n",
				args[1]), prompt_error("", NULL, data, 1), NULL);
	else
	{
		if (cwd() == 0)
		{
			pwd = get_env_value("PWD", data);
			if (pwd == NULL)
				prompt_error("cd: error retrieving current directory",
					NULL, data, 1);
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
		}
	}
	return (pwd);
}

char	*change_directory(char **args, t_data *data)
{
	char	*pwd;
	char	*next_pwd;

	if (strcmp(args[1], ".") == 0 || strcmp(args[1], "..") == 0
		|| strcmp(args[1], "./") == 0 || strcmp(args[1], "../") == 0)
	{
		pwd = current_parrent_dir(args, data);
		if (pwd == NULL)
			return (NULL);
	}
	else if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory\n",
				args[1]), prompt_error("", NULL, data, 1), NULL);
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
	return (pwd);
}

t_data	*ft_cd(t_params params, t_data *data)
{
	char	*pwd;
	char	**args;
	t_env	*env;

	env = data->linked_env;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	if (args[1] == NULL)
	{
		if (chdir(get_variable(env, "HOME")) == -1)
			return (prompt_error("cd: HOME not set", NULL, data, 1), NULL);
	}
	else if (ft_strncmp(args[1], "$HOME", 5) == 0)
	{
		if (chdir(args[1]) == -1)
			return (prompt_error("cd: HOME not set", NULL, data, 1), NULL);
	}
	else
	{
		pwd = change_directory(args, data);
		if (pwd == NULL)
			return (NULL);
	}
	return (data);
}
