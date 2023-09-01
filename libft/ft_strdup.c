/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strdup.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/12 16:31:55 by marvin 			  #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*ss;

	i = 1;
	while (s[i] != 0)
	{
		i++;
	}
	ss = (char *) malloc (sizeof(char) * i + 1); // tested
	if (!ss)
	{
		ft_printf("malloc error\n");
		return (0);
	}
	i = 0;
	while (s[i] != 0)
	{
		ss[i] = s[i];
		i++;
	}
	ss[i] = 0;
	return (ss);
}
