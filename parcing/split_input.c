/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 10:40:09 by hnait             #+#    #+#             */
/*   Updated: 2023/06/12 15:17:38 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	treat_input(t_data *data)
{
	int			i;
	int			param_len;

	param_len = 0;
	i = 0;
	handle_dollar(data);
	while (data->commande_line[i])
	{
		if (data->commande_line[i] == '\"' || data->commande_line[i] == '\'')
			handle_quotes(data, &i);
		else if (data->commande_line[i] == '<' || data->commande_line[i] == '>')
			add_operator(data, data->commande_line[i], &i);
		else if (data->commande_line[i] == '|')
			add_operator(data, '|', &i);
		else if (data->commande_line[i])
			handle_normal_char(data, &i, &param_len);
		if (data->commande_line[i] && data->commande_line[i] == ' ')
			while (data->commande_line[i] && data->commande_line[i] == ' ')
				i++;
	}
}

void	handle_normal_char(t_data *data, int *i, int *p_len)
{
	int		j;
	char	*param;

	j = *i;
	while (data->commande_line[j] && !is_operator(data->commande_line[j]))
	{
		// check dollar
		// if (data->commande_line[j] == '$')
		// 	handle_dollar(data->commande_line, &j, p_len, &data->params);
		// ft_printf("data->commande_line[j]: %c\n", data->commande_line[j]);
		j++;
		(*p_len)++;
	}
	param = ft_substr(data->commande_line, *i, *p_len);
	// ft_printf("data->commande_line: |%s|\n", data->commande_line);
	// ft_printf("param_len: %d\n", *p_len);
	// ft_printf("i: %d\n", *i);

	// ft_printf("param: |%s|\n", param);
	// sleep(1);
	param = handle_dollar_in_quotes(param, data);
	*i = j;
	*p_len = 0;
	add_param(&data->params, param);
}

char	*get_env_value(char *param, t_data *data)
{
	char	**env;
	int		param_len;

	param_len = ft_strlen(param);
	env = data->env;
	// ft_printf("param: |%s|\n", param);
	while (*env)
	{
		if (ft_strncmp(*env, param, param_len) == 0 && (*env)[param_len] == '=')
			return (free(param), ft_strdup(*env + param_len + 1));
		env++;
	}
	free(param);
	return (ft_strdup(""));
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc(ft_strlen(s1) + 2);
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

char	*expand_variable(char *param, int *i, t_data *data)
{
	int		j;
	char	*value;
	char	*new_param;

	j = ++*i;
	new_param = ft_strdup("");
	if (param[j] == '{')
	{
		while (param[j] && param[j] != '}')
			j++;
		if (param[j] == '}')
		{
			value = get_env_value(ft_substr(param, *i + 1, j - *i - 1), data);
			j++;
		}
		else
			value = ft_strdup("");
	}
	else
	{
		while (param[j] && !is_operator(param[j]) && param[j] != '$' && (ft_isalnum(param[j]) || param[j] == '_'))
			j++;
		value = get_env_value(ft_substr(param, *i, j - *i), data);
	}
	new_param = ft_strjoin(new_param, value, 1);
	*i = j;
	return (new_param);
}

char	*handle_dollar_in_quotes(char *param, t_data *data)
{
	int		i;
	char	*new_param;

	i = 0;
	new_param = ft_strdup("");
	while (param[i])
	{
		if (param[i] == '$')
		{
			new_param = expand_variable(param, &i, data);
		}
		else
		{
			new_param = ft_strjoin_char(new_param, param[i]);
		}
		i++;
	}
	free(param);
	return (new_param);
}

void	handle_dollar(t_data *data)
{
	int		i;
	int		j;
	char	*new_param;
	char	*new_command_line;
	char	quote;

	i = 0;
	j = 0;
	new_param = ft_strdup("");
	while (data->commande_line[i])
	{
		if (data->commande_line[i] == '\'')
		{
			quote = data->commande_line[i];
			while (data->commande_line[i] && data->commande_line[i] != quote)
				i++;
		}
		if (data->commande_line[i] == '$')
		{
			j = i;
			new_param = expand_variable(data->commande_line, &i, data);
			new_command_line = ft_calloc(ft_strlen(data->commande_line) - (i - j) + ft_strlen(new_param) + 1, sizeof(char));
			j = ft_strlcat(new_command_line, data->commande_line, j + 1);
			j = ft_strlcat(new_command_line, new_param, j + ft_strlen(new_param));
			j = ft_strlcat(new_command_line, data->commande_line, ft_strlen(data->commande_line) - i);
		}
		else
			i++;
	// ft_printf("done\n");
	}
	if (j)
		data->commande_line = new_command_line;
}

void	handle_quotes(t_data *data, int *i)
{
	int		j;
	char	*param;
	char	quote;
	int		lenght;

	quote = data->commande_line[*i];
	lenght = 0;
	j = ++(*i);
	while (data->commande_line[j] && data->commande_line[j] != quote) 
	{
		j++;
		lenght++;
	}
	if (!data->commande_line[j])
		prompt_error("Error: quote not closed");
	param = ft_substr(data->commande_line, *i, lenght);
	*i = j + 1;
	// if (quote == '\"')
	// 	param = handle_dollar_in_quotes(param, data);
	add_param(&data->params, param);
}
