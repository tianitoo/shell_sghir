/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pwd.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 17:09:30 by hnait			 #+#	#+#			 */
/*   Updated: 2023/09/11 00:23:00 by kmouradi		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_pwd(t_data *data)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cwd = get_env_value("PWD", data);
		if (cwd == NULL)
			return (NULL);
		pwd = ft_strdup(cwd);
	}
	else
	{
		pwd = ft_strdup(cwd);
		if (pwd == NULL)
			return (NULL);
		free(cwd);
	}
	return (pwd);
}

char	*ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = find_pwd(data);
	if (!cwd)
		return (NULL);
	ft_putstr_fd(cwd, 1);
	ft_printf("\n");
	free(cwd);
	return (NULL);
}
