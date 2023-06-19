/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_printf.h										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/13 17:19:14 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>
# include <limits.h>

int		ft_printf(const char *str, ...);
void	ft_putchar(char c, int *printed);
void	ft_putstr(char *s, int *printed);
void	ft_puthex(size_t num, char c, int *printed);
void	ft_putnbr(int n, int *printed);
void	ft_putunsignednbr(unsigned int n, int *printed);

#endif
