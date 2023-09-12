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

int	treat_input(t_data *data)
{
	int			i;
	int			param_len;

	param_len = 0;
	i = 0;
	while (data->commande_line[i])
	{
		if (!ft_isalnum(data->commande_line[i]) && data->commande_line[i] != ' ' && data->commande_line[i] != '\t' && data->commande_line[i] != '\"' && data->commande_line[i] != '\'' && data->commande_line[i] != '$' && data->commande_line[i] != '>' && data->commande_line[i] != '<' && data->commande_line[i] != '|' && data->commande_line[i] != '-')
			return (ft_printf("Error: special caracter: `%c", data->commande_line[i]), prompt_error("'", NULL, data, 1), 0);
		i++;
	}
	i = 0;
	if (handle_dollar(NULL, data) == NULL)
		return (0);
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
		{
			if (add_operator(data, '|', &i) == NULL)
				return (0);
		}
		if (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
			while (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
				i++;
	}
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

char	*handle_normal_char(t_data *data, int *i, int *p_len)
{
	int		j;
	int		k;
	char	*param;
	t_params	last_param;

	j = *i;
	k = -1;
	param = NULL;
	while (data->commande_line[j] && (!is_operator(data->commande_line[j]) || data->commande_line[j] == '\"' || data->commande_line[j] == '\''))
	{
		if (data->commande_line[j] == '\"' || data->commande_line[j] == '\'')
		{
			k = j;
			handle_quotes(data, &j);
			(*p_len) += j - k;
			if (is_operator(data->commande_line[j]))
				break;
		}
		else
		{
			j++;
			(*p_len)++;
		}
	}
	if (data->commande_line[j] == '\n')
		return (add_history(data->commande_line), prompt_error("Error: special caracter \\n", NULL, data, 1), NULL);
	param = ft_substr(data->commande_line, *i, *p_len);
	if (param == NULL)
		return (NULL);
	*i = j;
	*p_len = 0;
	if (add_param(&data->params, param, data) == NULL)
	{
		return (NULL);
	}
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
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	return (new_str);
}

char	*expand_variable(char *param, int *i, t_data *data)
{
	int		j;
	char	*tmp;
	char	*value;
	char	*new_param;

	j = ++*i;
	// ft_printf("param = %s\n", param[j]);
	new_param = ft_strdup("");
	if (new_param == NULL)
		return (NULL);
	// if (!ft_isalnum(data->commande_line[j]))
	// 	return (new_param)
	if (param[j] == '{')
	{
		while (param[j] && param[j] != '}')
			j++;
		if (param[j] == '}')
		{
			tmp = ft_substr(param, *i + 1, j - *i - 1);
			if (tmp == NULL)
				return (NULL);
			if (add_garbage(data, tmp) == NULL)
				return (NULL);
			value = get_env_value(tmp, data);
			if (value == NULL)
				return (NULL);
			j++;
		}
		else
		{
			value = ft_strdup("");
			if (value == NULL)
				return (NULL);
		}
		*i = j;
	}
	else
	{
		if (param[j] == '?')
		{
			value = ft_itoa(g_exit->g_exit_status);
			if (value == NULL)
				return (NULL);
			(*i)++;
		}
		else if (ft_isalpha(param[j]) || param[j] == '_')
		{
			while (param[j] && !is_operator(param[j]) && param[j] != '$' && (ft_isalpha(param[j]) || param[j] == '_'))
				j++;
			tmp = ft_substr(param, *i, j - *i);
			if (tmp == NULL)
				return (NULL);
			if (add_garbage(data, tmp) == NULL)
				return (NULL);
			value = get_env_value(tmp, data);
			if (value == NULL)
				return (NULL);
			*i = j;
		}
		else if (param[j] == '\"' || param[j] == '\'')
		{
			value = ft_strdup("");
			if (value == NULL)
				return (NULL);
		}
		else
		{
			(*i)++;
			value = ft_strdup("");
			if (value == NULL)
				return (NULL);
		}
	}
	new_param = ft_strjoin(new_param, value, 1);
	if (add_garbage(data, new_param) == NULL)
		return (NULL);
	return (new_param);
}

char	*handle_dollar_in_quotes(char *param, t_data *data)
{
	int		i;
	char	*new_param;

	i = 0;
	new_param = ft_strdup("");
	if (new_param == NULL)
		return (NULL);
	if (add_garbage(data, new_param) == NULL)
		return (NULL);
	while (param[i])
	{
		if (param[i] == '$')
		{
			new_param = expand_variable(param, &i, data);
			if (!new_param)
				return (NULL);
		}
		else
		{
			new_param = ft_strjoin_char(new_param, param[i], data);
			if (!new_param)
				return (NULL);
		}
		if (add_garbage(data, new_param) == NULL)
			return (NULL);
		i++;
	}
	return (new_param);
}

char	*handle_dollar(char **heredoc_input, t_data *data)
{
	int		i;
	int		in_quote;
	int		in_double_quotes;
	char	*command_line;
	char	*new_command_line;

	new_command_line = ft_strdup("");
	if (new_command_line == NULL)
		return (NULL);
	if (add_garbage(data, new_command_line) == NULL)
		return (NULL);
	in_quote = 0;
	in_double_quotes = 0;
	if (heredoc_input)
		command_line = *heredoc_input;
	else
		command_line = data->commande_line;
	i = 0;
	if (!command_line)
		return (NULL);
	while (command_line[i])
	{
		if (!in_quote && command_line[i] == '"')
			in_double_quotes = !in_double_quotes;
		if (!in_double_quotes && command_line[i] == '\'')
			in_quote = !in_quote;
		if (command_line[i] == '<' && command_line[i + 1] == '<' && !(in_double_quotes || in_quote))
		{
			i += 2;
			new_command_line = ft_strjoin_char(new_command_line, '<', data);
			if (!new_command_line)
				return (NULL);
			if (add_garbage(data, new_command_line) == NULL)
				return (NULL);
			new_command_line = ft_strjoin_char(new_command_line, '<', data);
			if (!new_command_line)
				return (NULL);
			if (add_garbage(data, new_command_line) == NULL)
				return (NULL);
			while (command_line[i] && command_line[i] == ' ')
			{
				new_command_line = ft_strjoin_char(new_command_line, ' ', data);
				if (!new_command_line)
					return (NULL);
				if (add_garbage(data, new_command_line) == NULL)
					return (NULL);
				i++;
			}
			if (command_line[i] == '$' && !(in_double_quotes && command_line[i + 1] == '"'))
			{
				new_command_line = ft_strjoin_char(new_command_line, '$', data);
				if (!new_command_line)
					return (NULL);
				if (add_garbage(data, new_command_line) == NULL)
					return (NULL);
				i++;
			}
		}
		if (!in_quote && command_line[i] == '$' && !(in_double_quotes && command_line[i + 1] == '"'))
		{
			char	*expanded_variable;

			expanded_variable = expand_variable(command_line, &i, data);
			if (!expanded_variable)
				return (NULL);
			new_command_line = ft_strjoin(new_command_line, expanded_variable, 0);
			if (!new_command_line)
				return (NULL);
		}
		else
		{
			new_command_line = ft_strjoin_char(new_command_line, command_line[i], data);
			if (!new_command_line)
				return (NULL);
			i++;
		}
		if (add_garbage(data, new_command_line) == NULL)
			return (NULL);
	}
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
