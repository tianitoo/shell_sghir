/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:51:48 by hnait             #+#    #+#             */
/*   Updated: 2023/09/18 04:56:49 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_normal_variable(char *param, int *i, t_data *data, int *j)
{
	char	*value;
	char	*tmp;

	while (param[*j] && !is_operator(param[*j]) && param[*j] != '$'
		&& (ft_isalpha(param[*j]) || param[*j] == '_'))
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

char	*add_quotes_to_word(char *str, char *new_str, int *i, t_data *data)
{
	new_str = ft_strjoin_char(new_str, '\"', data);
	if (new_str == NULL)
		return (NULL);
	while (str[*i] && str[*i] != ' ')
	{
		new_str = ft_strjoin_char(new_str, str[*i], data);
		if (new_str == NULL)
			return (NULL);
		(*i)++;
	}
	new_str = ft_strjoin_char(new_str, '\"', data);
	if (new_str == NULL)
		return (NULL);
	return (new_str);
}

char	*ft_quote(char *str, t_data *data)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = ft_strdup("");
	if (new_str == NULL)
		return (NULL);
	if (add_garbage(data, new_str) == NULL)
		return (NULL);
	while (str[i])
	{
		new_str = add_quotes_to_word(str, new_str, &i, data);
		while (str[i] && str[i] == ' ')
		{
			new_str = ft_strjoin_char(new_str, str[i], data);
			if (new_str == NULL)
				return (NULL);
			i++;
		}
	}
	return (new_str);
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
	if (g_exit->in_exec_mode == 1)
		value = ft_quote(value, data);
	new_param = ft_strjoin(new_param, value, 1);
	if (add_garbage(data, new_param) == NULL)
		return (NULL);
	return (new_param);
}
