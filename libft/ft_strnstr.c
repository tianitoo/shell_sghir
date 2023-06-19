/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strnstr.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/11 02:35:14 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int		i;
	int		intlen;
	int		ndlen;

	ndlen = ft_strlen(needle);
	i = 0;
	intlen = (int) len;
	if (haystack == 0 && len == 0)
	{
		return (0);
	}
	if (ndlen == 0 || needle == haystack)
		return ((char *)haystack);
	while (haystack[i] != 0 && i < intlen - ndlen)
	{
		if (ft_strncmp(haystack, needle, ndlen) == 0)
		{
			return ((char *) haystack);
		}
		i++;
		haystack++;
	}
	return (0);
}
