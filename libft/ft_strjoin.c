/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strjoin.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/09/15 16:20:41 by kmouradi		  #+#	#+#			 */
/*   Updated: 2023/09/15 16:21:11 by kmouradi		 ###   ########.fr	   */
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

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ss = (char *) ft_calloc (sizeof(char), (len_s1 + len_s2 + 1));
	if (!ss)
		return (NULL);
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
	return (ss);
}
