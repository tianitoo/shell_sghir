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

int	check_n(char *str)
{
	int	i;

	i = 1;
	if (ft_strncmp(str, "-n", 2) != 0)
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_arglen(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	skip_n_args(char **args, int *i, int *flag)
{
	while (args[*i])
	{
		if (check_n(args[*i]) == 1)
			*flag = 1;
		else
			break ;
		(*i)++;
	}
}

t_params	ft_echo(t_params params, t_data *data)
{
	char	**args;
	int		len;
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	args = args_to_double_pointer(params, data);
	if (args == NULL)
		return (NULL);
	len = ft_arglen(args) - 1;
	skip_n_args(args, &i, &flag);
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (i != len)
			ft_printf(" ");
		i++;
	}
	if (flag == 0)
		ft_printf("\n");
	g_exit->g_exit_status = 0;
	return (params);
}
