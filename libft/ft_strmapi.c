/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strmapi.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/21 19:36:50 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result_string;
	int		i;

	if (!s || !f)
		return (0);
	result_string = (char *) malloc (sizeof(char) * ft_strlen(s) + 1);
	if (!result_string)
		return (0);
	i = 0;
	while (s[i] != 0)
	{
		result_string[i] = (*f)(i, s[i]);
		i++;
	}
	result_string[i] = 0;
	return (result_string);
}
