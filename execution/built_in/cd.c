/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:01 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 19:34:39 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*current_parrent_dir(char **args, t_data *data)
{
	char	*pwd;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory  d",
				args[1]), prompt_error("", NULL, data, 1), NULL);
	else if (cwd(data) == 0)
	{
		pwd = get_env_value("PWD", data);
		if (garbage(pwd, data) == NULL)
			return (NULL);
		pwd = ft_strjoin(pwd, "/", 0);
		if (garbage(pwd, data) == NULL)
			return (NULL);
		pwd = ft_strjoin(pwd, args[1], 0);
		if (garbage(pwd, data) == NULL)
			return (NULL);
		if (update_env_var("PWD", pwd, data) == NULL)
			return (NULL);
	}
	return (old_pwd);
}

char	*move_to_dir(char **args, t_data *data)
{
	char	*pwd;

	pwd = find_pwd(data);
	if (pwd == NULL)
		return (NULL);
	if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory  d",
				args[1]), prompt_error("", NULL, data, 1), free(pwd), NULL);
	return (pwd);
}

char	*change_directory(char **args, t_data *data)
{
	char	*pwd;

	if (args == NULL)
		return (NULL);
	if (strcmp(args[1], ".") == 0 || strcmp(args[1], "..") == 0
		|| strcmp(args[1], "./") == 0 || strcmp(args[1], "../") == 0)
	{
		pwd = current_parrent_dir(args, data);
		if (pwd == NULL)
			return (NULL);
	}
	else
	{
		pwd = move_to_dir(args, data);
		if (pwd == NULL)
			return (NULL);
	}
	return (pwd);
}

char	*go_to_new_dir(char **args, t_env *env, t_data *data)
{
	char	*old_pwd;
	char	*home;

	if (args == NULL || args[1] == NULL)
	{
		old_pwd = getcwd(NULL, 0);
		home = get_variable(env, "HOME");
		if (home == NULL || chdir(home) == -1)
			return (prompt_error("cd: HOME not set", NULL, data, 1),
				free(old_pwd), NULL);
	}
	else
	{
		if (args == NULL)
			return (NULL);
		old_pwd = change_directory(args, data);
		if (old_pwd == NULL)
			return (NULL);
	}
	return (old_pwd);
}

t_data	*ft_cd(t_params params, t_data *data)
{
	char	*pwd;
	char	*old_pwd;
	char	**args;
	t_env	*env;

	env = data->linked_env;
	if (params->next && params->next->parameter[0] == '\0')
		return (data);
	args = NULL;
	if (params->next != NULL)
		args = args_to_double_pointer(params, data);
	old_pwd = go_to_new_dir(args, env, data);
	if (old_pwd == NULL)
		return (NULL);
	pwd = getcwd(NULL, 0);
	if (garbage(pwd, data) == NULL)
		return (NULL);
	if (*old_pwd)
		free(old_pwd);
	if (update_env_var("PWD", pwd, data) == NULL)
		return (NULL);
	return (data);
}
