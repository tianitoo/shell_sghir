/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:01 by hnait             #+#    #+#             */
/*   Updated: 2023/09/20 01:02:17 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*current_parrent_dir(char **args, t_data *data)
{
	char	*pwd;
	char	*new_pwd;

	pwd = NULL;
	new_pwd = NULL;
	if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory",
				args[1]), prompt_error("", NULL, data, 1), NULL);
	else if (cwd(data) == 0)
	{
		pwd = get_env_value("PWD", data);
		if (pwd == NULL)
			return (NULL);
		new_pwd = ft_strjoin(pwd, "/", 0);
		if (new_pwd == NULL)
			return (NULL);
		new_pwd = ft_strjoin(new_pwd, args[1], 1);
		if (new_pwd == NULL)
			return (NULL);
	}
	return (new_pwd);
}

char	*move_to_dir(char **args, t_data *data)
{
	char	*pwd;

	if (is_directory(args[1]) == 0)
		return (ft_printf("cd: %s: Not a directory", args[1]),
			prompt_error("", NULL, data, 1), NULL);
	if (chdir(args[1]) == -1)
		return (ft_printf("cd: %s: No such file or directory",
				args[1]), prompt_error("", NULL, data, 1), NULL);
	pwd = find_pwd(data);
	if (pwd == NULL)
		return (NULL);
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
	char	*pwd;
	char	*home;

	if (args == NULL || args[1] == NULL)
	{
		home = get_variable(env, "HOME");
		if (home == NULL || chdir(home) == -1)
			return (prompt_error("cd: HOME not set", NULL, data, 1),
				NULL);
		pwd = getcwd(NULL, 0);
	}
	else
	{
		pwd = change_directory(args, data);
		if (pwd == NULL)
			return (NULL);
	}
	return (pwd);
}

t_data	*ft_cd(t_params params, t_data *data)
{
	char	*pwd;
	char	**args;
	t_env	*env;

	env = data->linked_env;
	if (params->next && params->next->parameter[0] == '\0')
		return (data);
	args = NULL;
	if (params->next != NULL)
		args = args_to_double_pointer(params, data);
	pwd = go_to_new_dir(args, env, data);
	if (pwd == NULL)
		return (NULL);
	if (update_env_var("PWD", pwd, data) == NULL)
		return (NULL);
	free(pwd);
	return (data);
}
