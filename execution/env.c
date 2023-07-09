/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 15:54:13 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 15:54:15 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

void	write_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], 1);
			ft_printf('\n');
		}
		i++;
	}
}

int	check_slvl(char **env)
{
	int	i;

	i = 0;
	while (env[i++])
	{
		if (!ft_strncmp("SHLVL=", env[i], ft_strlen("SHLVL=")))
			return (1);
	}
	return (0);
}