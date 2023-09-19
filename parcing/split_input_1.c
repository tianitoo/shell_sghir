/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 04:22:48 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 02:34:00 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	normal_char_len(t_data *data, int *j, int *k, int *p_len)
{
	while (data->commande_line[*j] && (!is_operator(data->commande_line[*j]) 
			|| data->commande_line[*j] == '\"' 
			|| data->commande_line[*j] == '\''))
	{
		if (data->commande_line[*j] == '\"' || data->commande_line[*j] == '\'')
		{
			*k = *j;
			handle_quotes(data, j);
			(*p_len) += *j - *k;
			if (is_operator(data->commande_line[*j]))
				break ;
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
	int			j;
	int			k;
	char		*param;
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

void	handle_quotes(t_data *data, int *i)
{
	int		j;
	char	quote;
	int		lenght;

	quote = data->commande_line[*i];
	ft_memmove(&data->commande_line[*i], &data->commande_line[*i + 1],
		ft_strlen(&data->commande_line[*i + 1]));
	data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	lenght = 0;
	j = (*i);
	while (data->commande_line[j] && data->commande_line[j] != quote) 
	{
		j++;
		lenght++;
	}
	if (!data->commande_line[j])
		prompt_error("Error: quote not closed", NULL, data, 258);
	else
	{
		ft_memmove(&data->commande_line[j], &data->commande_line[j + 1],
			ft_strlen(&data->commande_line[j + 1]));
		data->commande_line[ft_strlen(data->commande_line) - 1] = 0;
	}
	*i = j;
}
