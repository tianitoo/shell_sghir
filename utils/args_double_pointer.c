/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   args_double_pointer.c							  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 14:29:14 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:39:30 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

char	**args_to_double_pointer(t_params params)
{
	int	 i;
	int		args_count;
	char	**args;
	t_params	tmp;

	i = 0;
	tmp = params;
	args_count = 0;
	while (tmp)
	{
		args_count++;
		tmp = tmp->next;
	}
	args = (char **)ft_calloc(sizeof(char *), (args_count + 1));
	while (params)
	{
		ft_printf("params->parameter = %s, i = %d\n", params->parameter, i);
		args[i] = ft_strdup(params->parameter);
		params = params->next;
		i++;
	}
	args[i] = NULL;
	return (args);

}