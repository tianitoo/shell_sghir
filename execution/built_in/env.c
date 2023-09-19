/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:23:10 by hnait             #+#    #+#             */
/*   Updated: 2023/09/20 00:14:10 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*write_env(t_data *data)
{
	t_env	*env;

	env = data->linked_env;
	if (env == NULL)
		return (NULL);
	while (env)
	{
		if (env->hidden == 0)
		{
			if (env->value != NULL && ft_strlen(env->value) > 0)
				ft_printf("%s=%s\n", env->key, env->value);
			else if (env->show_value == 1)
				ft_printf("%s=\n", env->key);
		}
		env = env->next;
	}
	return (data);
}

int	cwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (free(cwd),
			ft_printf("cd: error retrieving current directory: getcwd: cannot"),
			ft_printf(" access parent directories: No such file or directory"),
			prompt_error(
				"",
				NULL, data, 1), 0);
	free(cwd);
	return (1);
}

char	*find_key(char *str, t_data *data)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
		{
			key = ft_substr(str, 0, i);
			if (key == NULL)
				return (prompt_error("malloc error", NULL, data, 1), NULL);
			return (key);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (prompt_error("minishell: export: not a valid identifier",
					NULL, data, 1), NULL);
		i++;
	}
	key = ft_substr(str, 0, i);
	if (key == NULL)
		return (prompt_error("malloc error", NULL, data, 1), NULL);
	return (key);
}
