/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_putnbr.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 15:31:16 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr(int n, int *printed)
{
	char		c;
	long long	nbr;

	nbr = n;
	c = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		ft_putchar('-', printed);
	}
	if (nbr >= 0)
	{
		if (nbr < 10)
		{
			c = nbr + '0';
		}
		else
		{
			c = nbr % 10 + '0';
			nbr /= 10;
			ft_putnbr(nbr, printed);
		}
		if (*printed != -1)
			ft_putchar(c, printed);
	}
}
