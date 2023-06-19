/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_putstr.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 17:21:32 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putstr(char *s, int *printed)
{
	int	i;

	i = 0;
	if (s == NULL)
	{
		ft_putstr("(null)", printed);
	}
	else
		while (s[i] && *printed != -1)
			ft_putchar(s[i++], printed);
}
