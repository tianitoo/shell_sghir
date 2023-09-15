/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_itoa.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/17 21:17:16 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

int	nlen(int n)
{
	int		len;

	len = 0;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		len ++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	len = nlen(n);
	s = (char *) malloc (sizeof(char) * (len + 1));
	if (!s)
	{
		ft_printf("malloc error\n");
		return (0);
	}
	if (n < 0)
	{
		s[0] = '-';
		n = -n;
	}
	s[len] = '\0';
	while (len >= 0 && n > 0)
	{
		s[--len] = n % 10 + '0';
		n /= 10;
	}
	return (s);
}
