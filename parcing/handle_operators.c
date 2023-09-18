/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   handle_operators.c								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/09/11 00:52:56 by kmouradi		  #+#	#+#			 */
/*   Updated: 2023/09/15 16:17:40 by kmouradi		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}

t_params	add_operator(t_data *data, char operator, int *i)
{
	char		*param;
	t_params	tmp;

	param = get_operator(data->commande_line, i, operator);
	if (param == NULL)
		return (NULL);
	if (add_param(&(data->params), param, data) == NULL)
		return (NULL);
	tmp = get_last_param(data->params);
	tmp->is_operator = 1;
	return (tmp);
}

char	*get_operator(char *input, int *i, char operator)
{
	char	*param;

	if (operator == '<' && input[*i + 1] == '<')
	{
		param = ft_strdup("<<");
		(*i)++;
	}
	else if (operator == '<')
		param = ft_strdup("<");
	else if (operator == '>' && input[*i + 1] == '>')
	{
		param = ft_strdup(">>");
		(*i)++;
	}
	else if (operator == '>')
		param = ft_strdup(">");
	else
	{
		param = ft_substr(input, *i, 1);
	}
	(*i)++;
	if (param == NULL)
		return (NULL);
	return (param);
}
