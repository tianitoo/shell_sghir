/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   echo.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 15:54:00 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 15:54:01 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */


#include "../../minishell.h"

int ft_echo(t_params params)
{
	char	**args;
	int		i;
	int		newline;
	int		is_notn;
	int		j;

	args = args_to_double_pointer(params);
	is_notn = 0;
	newline = 0;
	i = 1;
	j = 1;
	if (args[i] != NULL && ft_strncmp(args[i], "-n", 2) == 0)
	{
		while (args[i][j] == 'n')
		{
			newline++;
			j++;
		}
		if (args[i][j] != '\0')
			is_notn = 1;
	}
	if (args[i] != NULL && newline > 0 && is_notn == 0)
		i++;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline == 0 || is_notn == 1)
		ft_printf("\n");
	return (0);
}

// int main(int ac, char **av)
// {
//	 // printf("%s\n",av[1]);
//	 ft_echo(av);
// }