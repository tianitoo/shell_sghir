/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strjoin.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/21 20:56:58 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char const *s2, int free_s1)
{
	char	*ss;
	int		i;
	int		j;
	int		len_s1;
	int		len_s2;

	if (!s1)
		return (0);
	if (!s2)
		return (0);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ss = (char *) malloc (sizeof(char) * (len_s1 + len_s2 + 1));
	if (!ss)
		return (0);
	i = -1;
	while (++i < len_s1)
		ss[i] = s1[i];
	j = -1;
	while (++j < len_s2)
		ss[i + j] = s2[j];
	if (free_s1)
	{
		free(s1);
		s1 = NULL;
	}
	ss[i + j] = 0;
	return (ss);
}
