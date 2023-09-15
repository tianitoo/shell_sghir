/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strcmp.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/09 14:21:13 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:22:26 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] != '\0' || str2[i] != '\0'))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	if ((str1[i] == '\0' && str2[i] != '\0')
		|| (str1[i] != '\0' && str2[i] == '\0'))
		return (str1[i] - str2[i]);
	return (0);
}
