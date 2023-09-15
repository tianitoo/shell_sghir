/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_printf.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 17:18:00 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

void	typetest(va_list vl, char c, int *printed)
{
	if (c == 'c')
		ft_putchar(va_arg(vl, int), printed);
	else if (c == '%')
		ft_putchar('%', printed);
	else if (c == 's')
		ft_putstr(va_arg(vl, char *), printed);
	else if (c == 'd' || c == 'i')
		ft_putnbr(va_arg(vl, int), printed);
	else if (c == 'u')
		ft_putunsignednbr(va_arg(vl, unsigned int), printed);
	else if (c == 'p' || c == 'x' || c == 'X')
	{
		if (c == 'p')
			ft_putstr("0x", printed);
		ft_puthex(va_arg(vl, size_t), c, printed);
	}
	else
		ft_putchar(c, printed);
}

int	ft_printf(const char *str, ...)
{
	va_list	vl;
	int		i;
	int		printed;

	i = 0;
	printed = 0;
	va_start(vl, str);
	while (str[i] && str)
	{
		if (str[i] == '%')
		{
			i++;
			typetest(vl, str[i], &printed);
		}
		else
			ft_putchar(str[i], &printed);
		if (printed == -1)
		{
			va_end(vl);
			return (-1);
		}
		i++;
	}
	va_end(vl);
	return (printed);
}
