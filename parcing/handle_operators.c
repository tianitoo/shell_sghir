
#include "../minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == ' ' || c == '\n')
		return (1);
	return (0);
}

void	add_operator(t_data *data, char operator, int *i)
{
	char		*param;
	t_params	tmp;

	param = get_operator(data->commande_line, i, operator);
	add_param(&(data->params), param);
	tmp = get_last_param(data->params);
	tmp->is_operator = 1;
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
		param = ft_substr(input, *i, 1);
	(*i)++;
	return (param);
}

t_params	new_param(char *param)
{
	t_params	new;

	new = malloc(sizeof(t_param));
	new->parameter = param;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	handle_params(t_cmd_list *cmd_list, t_data *data)
{
	t_cmd_list	tmp;
	t_params	params;

	(void)data;

	tmp = *cmd_list;
	while (tmp)
	{
		params = tmp->args;
		while (params && tmp->parsing_error == 0)
		{
			if ((params->parameter[0] == '>' || params->parameter[0] == '<') && params->is_operator == 1)
					handle_redirection(params, tmp, data);
			params = params->next;
		}
		if (tmp->cmd == NULL)
			tmp->cmd = ft_strdup(tmp->args->parameter);
		tmp = tmp->next;
	}
}

t_params	clone_t_params(t_params params)
{
	t_params	new;

	new = malloc(sizeof(t_param));
	new->parameter = ft_strdup(params->parameter);
	new->is_operator = params->is_operator;
	new->in_double_quote = params->in_double_quote;
	new->in_quote = params->in_quote;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_param_to_cmd(t_cmd_list cmd_list, t_params param)
{
	t_params	last_param;

	if (cmd_list->args == NULL)
		cmd_list->args = clone_t_params(param);
	else
	{
		last_param = get_last_param(cmd_list->args);
		last_param->next = clone_t_params(param);
		last_param->next->prev = last_param;
	}
}

t_cmd_list	get_cmd_list(t_data *data)
{
	t_params	handling_param;
	t_params	unhandled_params;
	t_cmd_list	cmd_list;
	t_cmd_list	head;
	
	unhandled_params = data->params;
	cmd_list = NULL;
	handling_param = unhandled_params;
	while (handling_param)
	{
		if (cmd_list == NULL)
		{
			if (handling_param->parameter[0] == '|' && handling_param->is_operator == 1)
			{
				prompt_error("syntax error near unexpected token `|'", NULL, data);
				return (NULL);
			}
			cmd_list = new_cmd(data);
			if (cmd_list == NULL)
				return (NULL);
			head = cmd_list;
		}
		if (handling_param->parameter[0] == '|' && handling_param->is_operator == 1)
		{
			if (handling_param->next == NULL)
			{
				prompt_error("syntax error near unexpected token `|'", NULL, data);
				return (NULL);
			}
			if (handling_param->next->is_operator == 1 && handling_param->next->parameter[0] == '|')
			{
				prompt_error("syntax error near unexpected token `||'", NULL, data);
				return (NULL);
			}
			cmd_list->next = new_cmd(data);
			cmd_list->next->prev = cmd_list;
			cmd_list = cmd_list->next;
			cmd_list->next = NULL;
		}
		else
			add_param_to_cmd(cmd_list, handling_param);
		if (handling_param->next == NULL)
			break ;
		handling_param = handling_param->next;
	}
	handle_params(&head, data);
	return (head);
}
