/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 22:17:45 by kmouradi          #+#    #+#             */
/*   Updated: 2023/09/19 02:34:13 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char(t_data *data, int *i)
{
	if (data->commande_line[*i] == '\"')
	{
		while (data->commande_line && data->commande_line[++(*i)] != '\"')
			if (data->commande_line[*i] == '\0'
				|| data->commande_line[*i] == '\n')
				return (ft_printf("Error: quote not closed"),
					prompt_error("'", NULL, data, 258), 0);
	}
	else if (data->commande_line[*i] == '\'')
	{
		while (data->commande_line && data->commande_line[++(*i)] != '\'')
			if (data->commande_line[*i] == '\0'
				|| data->commande_line[*i] == '\n')
				return (ft_printf("Error: quote not closed"),
					prompt_error("'", NULL, data, 258), 0);
	}
	else if (data->commande_line[*i] == '\n')
		return (ft_printf("Error: special caracter: `%c",
				data->commande_line[*i]),
			prompt_error("'", NULL, data, 1), 0);
	return (1);
}

int	check_special_char(t_data *data)
{
	int	i;

	i = 0;
	while (data->commande_line[i])
	{
		if (is_special_char(data, &i) == 0)
			return (0);
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
		while (data->commande_line[i] && (data->commande_line[i] == ' '
				|| data->commande_line[i] == '\t'))
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
