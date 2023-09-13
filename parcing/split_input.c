/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   split_input.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/02 10:40:09 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 14:55:42 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int	check_special_char(t_data *data)
{
	int	i;

	i = 0;
	while (data->commande_line[i])
	{
		if (data->commande_line[i] == '\"')
		{
			while (data->commande_line && data->commande_line[++i] != '\"')
				if (data->commande_line[i] == '\0')
					return (ft_printf("Error: quote not closed"), prompt_error("'", NULL, data, 1), 0);
		}
		else if (data->commande_line[i] == '\'')
		{
			while (data->commande_line && data->commande_line[++i] != '\'')
				if (data->commande_line[i] == '\0')
					return (ft_printf("Error: quote not closed"), prompt_error("'", NULL, data, 1), 0);
		}
		else if (!ft_isalnum(data->commande_line[i]) && data->commande_line[i] != ' ' && data->commande_line[i] != '\t' && data->commande_line[i] != '\"' && data->commande_line[i] != '\'' && data->commande_line[i] != '$' && data->commande_line[i] != '>' && data->commande_line[i] != '<' && data->commande_line[i] != '|' && data->commande_line[i] != '-' && data->commande_line[i] != '+' && data->commande_line[i] != '=' && data->commande_line[i] != '.' && data->commande_line[i] != '/' && data->commande_line[i] != '/')
			return (ft_printf("Error: special caracter: `%c", data->commande_line[i]), prompt_error("'", NULL, data, 1), 0);
		i++;
	}
	return (1);
}

int	create_params(t_data *data)
{
	int		i;
	int		param_len;

	i = 0;
	param_len = 0;
	while (data->commande_line[i])
	{
		if (data->commande_line[i] && !is_operator(data->commande_line[i]))
		{
			if (handle_normal_char(data, &i, &param_len) == NULL)
				return (0);
		}
		else if (data->commande_line[i] == '<' || data->commande_line[i] == '>')
		{
			if (add_operator(data, data->commande_line[i], &i) == NULL)
				return (0);
		}
		else if (data->commande_line[i] == '|')
			if (add_operator(data, '|', &i) == NULL)
				return (0);
		if (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
			while (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
				i++;
	}
	return (1);
}

int	treat_input(t_data *data)
{
	if (check_special_char(data) == 0)
		return (0);
	if (handle_dollar(NULL, data) == NULL)
		return (0);
	if (create_params(data) == 0)
		return (0);
	return (1);
}

t_params	get_last_param(t_params params)
{
	t_params	tmp;

	tmp = params;
	if (tmp == NULL)
		return (NULL);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}
int	normal_char_len(t_data *data, int *j, int *k, int *p_len)
{

	while (data->commande_line[*j] && (!is_operator(data->commande_line[*j]) || data->commande_line[*j] == '\"' || data->commande_line[*j] == '\''))
	{
		if (data->commande_line[*j] == '\"' || data->commande_line[*j] == '\'')
		{
			*k = *j;
			handle_quotes(data, j);
			(*p_len) += *j - *k;
			if (is_operator(data->commande_line[*j]))
				break;
		}
		else
		{
			(*j)++;
			(*p_len)++;
		}
	}
	return (1);
}

char	*handle_normal_char(t_data *data, int *i, int *p_len)
{
	int		j;
	int		k;
	char	*param;
	t_params	last_param;

	j = *i;
	k = -1;
	param = NULL;
	normal_char_len(data, &j, &k, p_len);
	param = ft_substr(data->commande_line, *i, *p_len);
	if (param == NULL)
		return (NULL);
	*i = j;
	*p_len = 0;
	if (add_param(&data->params, param, data) == NULL)
		return (NULL);
	last_param = get_last_param(data->params);
	last_param->is_operator = 0;
	if (k != -1)
	{
		last_param->in_quote = 1;
		last_param->in_double_quote = 1;
	}
	return (param);
}

char	*get_env_value(char *param, t_data *data)
{
	t_env	*env;
	char	*value;
	char	*key;

	env = data->linked_env;
	while (env)
	{
		key = env->key;
		if (ft_strcmp(key, param) == 0)
		{
			value = env->value;
			return (value);
		}
		env = env->next;
	}
	value = ft_strdup("");
	if (value == NULL)
		return (NULL);
	if (add_garbage(data, value) == NULL)
		return (NULL);
	return (value);
}

char	*ft_strjoin_char(char *s1, char c, t_data *data)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc(ft_strlen(s1) + 2); // tested
	if (!new_str)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);
	}
	if (add_garbage(data, new_str) == NULL)
		return (NULL);
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	return (new_str);
}

char	*expand_normal_variable(char *param, int *i, t_data *data, int *j)
{
	char	*value;
	char	*tmp;
	while (param[*j] && !is_operator(param[*j]) && param[*j] != '$' && (ft_isalpha(param[*j]) || param[*j] == '_'))
		(*j)++;
	tmp = ft_substr(param, *i, *j - *i);
	if (tmp == NULL)
		return (NULL);
	if (add_garbage(data, tmp) == NULL)
		return (NULL);
	value = get_env_value(tmp, data);
	if (value == NULL)
		return (NULL);
	*i = *j;
	return (value);
}

char	*expand_special_variable(char *param, int *i, t_data *data, int *j)
{
	char	*value;

	if (param[*j] == '?')
	{
		value = ft_itoa(g_exit->g_exit_status);
		if (value == NULL)
			return (NULL);
		(*i)++;
	}
	else
	{
		if (param[*j] == '\"' || param[*j] == '\'')
			(*i)++;
		value = ft_strdup("");
		if (value == NULL)
			return (NULL);
	}
	if (add_garbage(data, value) == NULL)
		return (NULL);
	return (value);
}

char	*expand_variable(char *param, int *i, t_data *data)
{
	int		j;
	char	*value;
	char	*new_param;

	j = ++*i;
	new_param = ft_strdup("");
	if (new_param == NULL)
		return (NULL);
	if (ft_isalpha(param[j]) || param[j] == '_')
		value = expand_normal_variable(param, i, data, &j);
	else
		value = expand_special_variable(param, i, data, &j);
	if (value == NULL)
		return (NULL);
	new_param = ft_strjoin(new_param, value, 1);
	if (add_garbage(data, new_param) == NULL)
		return (NULL);
	return (new_param);
}

char *expand_join_variable(char *command_line, char *new_command_line, int *i, t_data *data)
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

char	*skip_spaces(char *command_line, char *new_command_line, int *i, t_data *data)
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

void	*garbage(void *garbage, t_data *data)
{
	if (garbage == NULL)
		return (NULL);
	if (add_garbage(data, garbage) == NULL)
		return (NULL);
	return (garbage);
}

char	*heredoc_delimiter(char *command_line, int *i, t_data *data, int in_double_quotes)
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
	if (command_line[*i] == '$' && !(in_double_quotes && command_line[*i + 1] == '"'))
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
	if (command_line[*i] == '<' && command_line[*i + 1] == '<' && !(quotes[1] || quotes[0]))
	{
		new_command_line = heredoc_delimiter(command_line, i, data, quotes[1]);
		if (new_command_line == NULL)
			return (NULL);
	}
	return (new_command_line);
}

char	*expand(int *i, t_data *data, char *new_command_line, int *quotes)
{
	char	*command_line;

	command_line = data->commande_line;
	if (!quotes[0] && command_line[*i] == '$' && !(quotes[1] && command_line[*i + 1] == '"'))
	{
		new_command_line = expand_join_variable(command_line, new_command_line, i, data);
		if (garbage(new_command_line, data) == NULL)
			return (NULL);
	}
	else
	{
		new_command_line = ft_strjoin_char(new_command_line, command_line[*i], data);
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
		new_command_line = ft_strjoin(new_command_line, check_heredoc(command_line, &i, data, quotes), 0);
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

void	handle_quotes(t_data *data, int *i)
{
	int		j;
	char	quote;
	int		lenght;

	quote = data->commande_line[*i];
	ft_memmove(&data->commande_line[*i], &data->commande_line[*i + 1], ft_strlen(&data->commande_line[*i + 1]));
	data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	lenght = 0;
	j = (*i);
	while (data->commande_line[j] && data->commande_line[j] != quote) 
	{
		j++;
		lenght++;
	}
	if (!data->commande_line[j])
		prompt_error("Error: quote not closed", NULL, data, 1);
	else
	{
		ft_memmove(&data->commande_line[j], &data->commande_line[j + 1], ft_strlen(&data->commande_line[j + 1]));
		data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	}
	*i = j;
}
