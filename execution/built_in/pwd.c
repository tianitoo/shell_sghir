/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmouradi <kmouradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 17:09:30 by hnait             #+#    #+#             */
/*   Updated: 2023/09/11 00:23:00 by kmouradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cwd = get_env_value("PWD", data);
		if (cwd == NULL)
			return (NULL);
	}
	if (ft_strlen(cwd) == 0)
	{
		return (get_env_value("HIDDEN_PWD", data));
	}
	if (key_exists(data->linked_env, "HIDDEN_PWD") == -1)
		return (NULL);
	else if (key_exists(data->linked_env, "HIDDEN_PWD") == 1)
	{
		if (update_env_var("HIDDEN_PWD", cwd, data) == NULL)
			return (NULL);
	}
	else
	{
		add_hidden_env(data->linked_env, "HIDDEN_ENV", cwd, data);
	}
	return (cwd);
}

char	*ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = find_pwd(data);
	if (!cwd)
		return (NULL);
	ft_putstr_fd(cwd, 1);
	ft_printf("\n");
	cwd = NULL;
	return (cwd);
}
