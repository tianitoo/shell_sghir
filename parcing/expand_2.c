/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:53:02 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:53:14 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_join_variable(char *command_line, char *new_command_line,
	int *i, t_data *data)
{
	char	*expanded_variable;

	expanded_variable = expand_variable(command_line, i, data);
	if (!expanded_variable)
		return (NULL);
	new_command_line = ft_strjoin(new_command_line, expanded_variable, 0);
	if (!new_command_line)
		return (NULL);
	return (new_command_line);
}

char	*skip_spaces(char *command_line, char *new_command_line,
	int *i, t_data *data)
{
	while (command_line[*i] && command_line[*i] == ' ')
	{
		new_command_line = ft_strjoin_char(new_command_line, ' ', data);
		if (!new_command_line)
			return (NULL);
		(*i)++;
	}
	return (new_command_line);
}

char	*heredoc_delimiter(char *command_line, int *i,
	t_data *data, int in_double_quotes)
{
	char	*new_command_line;

	*i += 2;
	new_command_line = ft_strdup("");
	if (garbage(new_command_line, data) == NULL)
		return (NULL);
	new_command_line = ft_strjoin_char(new_command_line, '<', data);
	if (new_command_line == NULL)
		return (NULL);
	new_command_line = ft_strjoin_char(new_command_line, '<', data);
	if (new_command_line == NULL)
		return (NULL);
	new_command_line = skip_spaces(command_line, new_command_line, i, data);
	if (command_line[*i] == '$' && !(in_double_quotes
			&& command_line[*i + 1] == '"'))
	{
		new_command_line = ft_strjoin_char(new_command_line, '$', data);
		if (new_command_line == NULL)
			return (NULL);
		i++;
	}
	return (new_command_line);
}

void	check_quote(char *command_line, int *i, int *quotes)
{
	if ((!quotes[0] && command_line[*i] == '"'))
		quotes[1] = !quotes[1];
	if ((!quotes[1] && command_line[*i] == '\''))
		quotes[0] = !quotes[0];
}

char	*check_heredoc(char *command_line, int *i, t_data *data, int *quotes)
{
	char	*new_command_line;

	new_command_line = ft_strdup("");
	if (garbage(new_command_line, data) == NULL)
		return (NULL);
	if (command_line[*i] == '<' && command_line[*i + 1] == '<'
		&& !(quotes[1] || quotes[0]))
	{
		new_command_line = heredoc_delimiter(command_line, i, data, quotes[1]);
		if (new_command_line == NULL)
			return (NULL);
	}
	return (new_command_line);
}
