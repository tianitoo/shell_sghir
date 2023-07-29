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

void	treat_input(t_data *data)
{
	int			i;
	int			param_len;

	param_len = 0;
	i = 0;
	handle_dollar(NULL, data);
	while (data->commande_line[i])
	{
		if (data->commande_line[i] && !is_operator(data->commande_line[i]))
			handle_normal_char(data, &i, &param_len);
		else if (data->commande_line[i] == '<' || data->commande_line[i] == '>')
			add_operator(data, data->commande_line[i], &i);
		else if (data->commande_line[i] == '|')
			add_operator(data, '|', &i);
		if (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
			while (data->commande_line[i] && (data->commande_line[i] == ' ' || data->commande_line[i] == '\t'))
				i++;
		// i++;
	}
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

void	handle_normal_char(t_data *data, int *i, int *p_len)
{
	int		j;
	int		k;
	char	*param;
	t_params	last_param;

	j = *i;
	k = -1;
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
	param = ft_substr(data->commande_line, *i, *p_len);
	*i = j;
	*p_len = 0;
	add_param(&data->params, param);
	last_param = get_last_param(data->params);
	last_param->is_operator = 0;
	if (k != -1)
	{
		last_param->in_quote = 1;
		last_param->in_double_quote = 1;
	}
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
	add_garbage(value);
	return (value);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc(ft_strlen(s1) + 2);
	if (!new_str)
		return (NULL);
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	// free(s1);
	add_garbage(new_str);
	return (new_str);
}

char	*expand_variable(char *param, int *i, t_data *data)
{
	int		j;
	char	*tmp;
	char	*value;
	char	*new_param;

	j = ++*i;
	new_param = ft_strdup("");
	add_garbage(new_param);
	if (param[j] == '{')
	{
		while (param[j] && param[j] != '}')
			j++;
		if (param[j] == '}')
		{
			tmp = ft_substr(param, *i + 1, j - *i - 1);
			add_garbage(tmp);
			value = get_env_value(tmp, data);
			j++;
		}
		else
			value = ft_strdup("");
		*i = j;
	}
	else
	{
		if (ft_isalnum(param[j]) || param[j] == '_')
		{
			while (param[j] && !is_operator(param[j]) && param[j] != '$' && (ft_isalnum(param[j]) || param[j] == '_'))
				j++;
			tmp = ft_substr(param, *i, j - *i);
			add_garbage(tmp);
			value = get_env_value(tmp, data);
			*i = j;
		}
		else
		{
			(*i)++;
			value = ft_strdup("$");
		}
	}
	new_param = ft_strjoin(new_param, value, 1);
	add_garbage(value);
	add_garbage(new_param);
	return (new_param);
}

char	*handle_dollar_in_quotes(char *param, t_data *data)
{
	int		i;
	char	*new_param;

	i = 0;
	new_param = ft_strdup("");
	add_garbage(new_param);
	while (param[i])
	{
		if (param[i] == '$')
		{
			new_param = expand_variable(param, &i, data);
		}
		else
		{
			new_param = ft_strjoin_char(new_param, param[i]);
			if (!new_param)
				prompt_error("Error: malloc failed", NULL, data);
		}
		i++;
	}
	// free(param);
	return (new_param);
}

void	handle_dollar(char **heredoc_input, t_data *data)
{
	int		i;
	int		in_quote;
	int		in_double_quotes;
	char	*command_line;
	char	*new_command_line;

	new_command_line = ft_strdup("");
	add_garbage(new_command_line);
	in_quote = 0;
	in_double_quotes = 0;
	if (heredoc_input)
		command_line = *heredoc_input;
	else
		command_line = data->commande_line;
	i = 0;
	while (command_line[i])
	{
		if (!in_quote && command_line[i] == '"')
			in_double_quotes = !in_double_quotes;
		if (!in_double_quotes && command_line[i] == '\'')
			in_quote = !in_quote;
		if (!in_quote && command_line[i] == '$' && !(in_double_quotes && command_line[i + 1] == '"'))
		{
			new_command_line = ft_strjoin(new_command_line, expand_variable(command_line, &i, data), 1);
			add_garbage(new_command_line);
		}
		else
		{
			new_command_line = ft_strjoin_char(new_command_line, command_line[i]);
			i++;
		}
	}
	if (heredoc_input)
		*heredoc_input = new_command_line;
	else
		data->commande_line = new_command_line;
}

void	handle_quotes(t_data *data, int *i)
{
	int		j;
	char	quote;
	int		lenght;

	quote = data->commande_line[*i];
	// ft_printf("quote: %c\n", quote);
	ft_memmove(&data->commande_line[*i], &data->commande_line[*i + 1], ft_strlen(&data->commande_line[*i + 1]));
	data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	lenght = 0;
	j = (*i);
	while (data->commande_line[j] && data->commande_line[j] != quote) 
	{
		// ft_printf("char: %c\n", data->commande_line[j]);
		j++;
		lenght++;
	}
	if (!data->commande_line[j])
		prompt_error("Error: quote not closed", NULL, data);
	else
	{
		ft_memmove(&data->commande_line[j], &data->commande_line[j + 1], ft_strlen(&data->commande_line[j + 1]));
		data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	}
	// data->commande_line[j] = 0;
	*i = j;
}
