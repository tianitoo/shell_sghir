/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:28:30 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:57:04 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*not_expandable(char *command_line, int *i, t_data *data,
	char *new_command_line)
{
	if (command_line[*i] == '$' && !(ft_isalpha(command_line[*i + 1])
			|| command_line[*i + 1] == '_' || command_line[*i + 1] == '?'))
	{
		(*i)++;
		new_command_line = ft_strjoin_char(new_command_line, '$', data);
		if (new_command_line == NULL)
			return (NULL);
	}
	return (new_command_line);
}

char	*expand(int *i, t_data *data, char *new_command_line, int *quotes)
{
	char	*command_line;

	command_line = data->commande_line;
	new_command_line = not_expandable(command_line, i, data, new_command_line);
	if (new_command_line == NULL)
		return (NULL);
	else if (!quotes[0] && command_line[*i] == '$' && !(quotes[1]
			&& command_line[*i + 1] == '"'))
	{
		new_command_line = expand_join_variable(command_line,
				new_command_line, i, data);
		if (garbage(new_command_line, data) == NULL)
			return (NULL);
	}
	else
	{
		new_command_line = ft_strjoin_char(new_command_line,
				command_line[*i], data);
		if (new_command_line == NULL)
			return (NULL);
		(*i)++;
	}
	return (new_command_line);
}

char	*expand_command_line(char *command_line, t_data *data)
{
	int		i;
	int		*quotes;
	char	*new_command_line;

	i = 0;
	quotes = ft_calloc(sizeof(int), 2);
	if (garbage(quotes, data) == NULL)
		return (NULL);
	new_command_line = ft_strdup("");
	if (garbage(new_command_line, data) == NULL)
		return (NULL);
	while (command_line[i])
	{
		check_quote(command_line, &i, quotes);
		new_command_line = ft_strjoin(new_command_line,
				check_heredoc(command_line, &i, data, quotes), 0);
		if (garbage(new_command_line, data) == NULL)
			return (NULL);
		new_command_line = expand(&i, data, new_command_line, quotes);
	}
	return (new_command_line);
}

char	*handle_dollar(char **heredoc_input, t_data *data)
{
	char	*command_line;
	char	*new_command_line;

	new_command_line = ft_strdup("");
	if (new_command_line == NULL)
		return (NULL);
	if (add_garbage(data, new_command_line) == NULL)
		return (NULL);
	if (heredoc_input)
	{
		command_line = *heredoc_input;
		data->commande_line = *heredoc_input;
	}
	else
		command_line = data->commande_line;
	if (!command_line)
		return (NULL);
	new_command_line = expand_command_line(command_line, data);
	if (heredoc_input)
		*heredoc_input = new_command_line;
	else
		data->commande_line = new_command_line;
	return (new_command_line);
}
