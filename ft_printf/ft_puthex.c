/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_puthex.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 15:31:12 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_puthex(size_t num, char c, int *printed)
{
	if (c != 'p')
		num = (unsigned int)num;
	if (num >= 16)
	{
		ft_puthex(num / 16, c, printed);
		ft_puthex(num % 16, c, printed);
	}
	else
	{
		if (*printed != -1)
		{
			if (num <= 9)
				ft_putchar(num + '0', printed);
			else
			{
				if (c == 'X')
					ft_putchar(num - 10 + 'A', printed);
				else
					ft_putchar(num - 10 + 'a', printed);
			}
		}
	}
}
