
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
	else{
		param = ft_substr(input, *i, 1);}
	(*i)++;
	if (param == NULL)
		return (NULL);
	return (param);
}

t_params	new_param(char *param, t_data *data)
{
	t_params	new;

	new = malloc(sizeof(t_param)); // tested
	if (new == NULL)
	{
		prompt_error("malloc error 1", NULL, data, 1);
		return (NULL);
	}
	new->parameter = param;
	new->next = NULL;
	new->prev = NULL;
	new->is_operator = 0;
	new->in_double_quote = -1;
	new->in_quote = -1;
	// add_garbag//e(new);
	return (new);
}

int	handle_params(t_cmd_list *cmd_list, t_data *data)
{
	t_cmd_list	tmp;
	t_params	params;

	tmp = *cmd_list;
	while (tmp)
	{
		params = tmp->args;
		while (params && tmp->parsing_error == 0 && data->parsing_error == 0)
		{
			if ((params->parameter[0] == '>' || params->parameter[0] == '<') && params->is_operator == 1)
				if (handle_redirection(params, tmp, data) == 0)
					return (0);
			params = params->next;
		}
		if (tmp->cmd == NULL)
			if (tmp->args && tmp->args->parameter)
			{
				tmp->cmd = ft_strdup(tmp->args->parameter);
				if (add_garbage(data, tmp->cmd) == NULL)
					return (0);
			}
		tmp = tmp->next;
	}
	return (1);
}

t_params	clone_t_params(t_params params, t_data *data)
{
	t_params	new;

	new = malloc(sizeof(t_param)); // tested
	if (new == NULL)
	{
		prompt_error("malloc error 8", NULL, data, 1);
		return (NULL);
	}
	if (add_garbage(data, new) == NULL)
		return (NULL);
	new->parameter = ft_strdup(params->parameter);
	if (new->parameter == NULL)
		return (NULL);
	if (add_garbage(data, new->parameter) == NULL)
		return (NULL);
	new->is_operator = params->is_operator;
	new->in_double_quote = params->in_double_quote;
	new->in_quote = params->in_quote;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_params	add_param_to_cmd(t_cmd_list cmd_list, t_params param, t_data *data)
{
	t_params	last_param;

	if (cmd_list->args == NULL)
	{
		cmd_list->args = clone_t_params(param, data);
		if (cmd_list->args == NULL)
			return (NULL);
	}
	else
	{
		last_param = get_last_param(cmd_list->args);
		last_param->next = clone_t_params(param, data);
		if (last_param->next == NULL)
			return (NULL);
	last_param->next->prev = last_param;
	}
	return (cmd_list->args);
}

t_cmd_list	first_command(t_cmd_list cmd_list, t_params handling_param, t_data *data)
{
	if (handling_param->parameter[0] == '|' && handling_param->is_operator == 1)
		return (prompt_error("syntax error near unexpected token `|'", NULL, data, 258), NULL);
	cmd_list = new_cmd(data);
	if (cmd_list == NULL)
		return (NULL);
	return (cmd_list);
}

t_cmd_list	next_command(t_cmd_list cmd_list, t_params handling_param, t_data *data)
{
	if (handling_param->next == NULL)
		return (prompt_error("syntax error near unexpected token `|'", NULL, data, 258), NULL);
	if (handling_param->next->is_operator == 1 && handling_param->next->parameter[0] == '|')
		return (prompt_error("syntax error near unexpected token `|'", NULL, data, 258), NULL);
	cmd_list->next = new_cmd(data);
	cmd_list->next->prev = cmd_list;
	cmd_list = cmd_list->next;
	cmd_list->next = NULL;
	return (cmd_list);
}

t_cmd_list	add_cmd_list(t_cmd_list cmd_list, t_params handling_param, t_data *data)
{
	if (cmd_list == NULL)
	{
		cmd_list = first_command(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
	}
	if (handling_param->parameter[0] == '|' && handling_param->is_operator == 1)
	{
		cmd_list = next_command(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
	}
	else
		if (add_param_to_cmd(cmd_list, handling_param, data) == NULL)
			return (NULL);
	return (cmd_list);
}

t_cmd_list	get_cmd_list(t_data *data)
{
	t_params	handling_param;
	t_cmd_list	cmd_list;
	t_cmd_list	head;

	handling_param = data->params;
	cmd_list = NULL;
	while (handling_param)
	{
		cmd_list = add_cmd_list(cmd_list, handling_param, data);
		if (cmd_list == NULL)
			return (NULL);
		if (head == NULL)
			head = cmd_list;
		if (handling_param->next == NULL)
			break ;
		handling_param = handling_param->next;
	}
	if (data->parsing_error == 0)
	{	
		if (handle_params(&head, data) == 0)
			return (NULL);
	}
	return (head);
}
