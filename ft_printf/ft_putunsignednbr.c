/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_putunsignednbr.c								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 17:22:05 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putunsignednbr(unsigned int n, int *printed)
{
	char	c;

	if (n == 0)
		c = '0';
	if (n > 0)
	{
		c = n % 10 + '0';
		n /= 10;
	}
	if (n != 0)
		ft_putunsignednbr(n, printed);
	if (*printed != -1)
		ft_putchar(c, printed);
}
