/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_split.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/17 21:33:36 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

int	sslen(char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (*s)
	{
		if (*s == c)
			i++;
		else
			len++;
		s++;
	}
	return (len);
}

void	free_ss(char **ss)
{
	while (*ss)
	{
		free(*ss);
		*ss = NULL;
		ss++;
	}
}

int	next_sep(const char *s, int j, char c)
{
	while (s[j] != c && s[j])
		j++;
	return (j);
}

char	*cut_word(const char *s, int *j, char c)
{
	char	*word;
	int		next;

	next = next_sep(s, *j, c);
	word = ft_substr(s, *j, next - *j);
	if (word == NULL)
		return (NULL);
	*j = next;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**ss;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	ss = (char **) malloc (sizeof(char *) * sslen(s, c) + 1);
	if (!ss)
		return (0);
	while (s[j] != 0)
	{
		if (s[j] == c)
			j++;
		else
		{
			ss[i] = cut_word(s, &j, c);
			if (!ss[i])
				free_ss(ss);
			i++;
		}
	}
	ss[i] = 0;
	return (ss);
}
