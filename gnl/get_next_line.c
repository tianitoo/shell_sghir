/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 11:48:05 by hnait             #+#    #+#             */
/*   Updated: 2022/12/15 11:00:15 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_nl_index(char **next_line, char *buffer)
{
	int		i;
	char	*tmp_nl;

	i = 0;
	tmp_nl = ft_strjoin(*next_line, buffer, 0);
	if (!tmp_nl)
		return (-2);
	free(*next_line);
	*next_line = tmp_nl;
	while ((*next_line)[i] != '\0')
	{
		if ((*next_line)[i] == '\n')
			return (i + 1);
		i++;
	}
	return (-1);
}

int	find_nl(char **next_line, int fd)
{
	char	*buffer;
	int		i;
	int		eof;

	eof = 1;
	buffer = (char *)ft_calloc(sizeof(char), 10 + 1);
	if (!buffer)
		return (-1);
	if (ft_strlen(*next_line) == 0)
		eof = read(fd, buffer, 10);
	while (eof > 0)
	{
		i = find_nl_index(next_line, buffer);
		if (i >= 0)
			return (free(buffer), i);
		if (i == -2)
			return (free(buffer), -1);
		eof = read(fd, buffer, 10);
	}
	if (eof == -1)
		return (free(buffer), -1);
	return (free(buffer), ft_strlen(*next_line));
}

void	remove_nl(char *next_line, int nl_index)
{
	int	i;

	i = 0;
	while (next_line[nl_index] != '\0')
		next_line[i++] = next_line[nl_index++];
	while (next_line[i])
		next_line[i++] = '\0';
}

char	*get_next_line(int fd)
{
	static char	*next_line;
	char		*return_value;
	int			nl_index;

	if (fd < 0 || 10 <= 0)
		return (NULL);
	if (!next_line)
	{
		next_line = (char *)ft_calloc(sizeof(char), 1);
		if (!next_line)
			return (NULL);
	}
	nl_index = find_nl(&next_line, fd);
	if (nl_index < 0 || ft_strlen(next_line) <= 0)
		return (free(next_line), next_line = NULL, NULL);
	return_value = ft_substr(next_line, nl_index, 0);
	if (!return_value)
		return (free(next_line), next_line = NULL, NULL);
	remove_nl(next_line, nl_index);
	return (return_value);
}
