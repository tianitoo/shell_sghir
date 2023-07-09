
#include "../minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == ' ' || c == '\n')
		return (1);
	return (0);
}

void	add_operator(t_data *data, char operator, int *i)
{
	char	*param;

	ft_printf("%c\n", operator);
	param = get_operator(data->commande_line, i, operator);
	add_param(&(data->params), param);
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
	ft_printf("hello\n");
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

void	handle_params(t_cmd_list *cmd_list)
{
	t_cmd_list	tmp;
	t_params	params;

	tmp = *cmd_list;
	while (tmp)
	{
		params = tmp->args;
		while (params)
		{
			if (params->parameter[0] == '>' || params->parameter[0] == '<')
				handle_redirection(params, tmp);
			params = params->next;
		}
		tmp = tmp->next;
	}
}

t_cmd_list	get_cmd_list(t_params params)
{
	t_params	tmp;
	t_cmd_list	cmd_list;
	t_cmd_list	head;

	cmd_list = NULL;
	tmp = params;
	while (tmp)
	{
		if (cmd_list == NULL)
		{
			if (tmp->parameter[0] == '|')
			{
				prompt_error("syntax error near unexpected token `|'");
				return (NULL);
			}
			cmd_list = new_cmd();
			if (cmd_list == NULL)
				return (NULL);
			head = cmd_list;
		}
		if (tmp->parameter[0] == '|')
		{
			cmd_list->next = new_cmd();
			cmd_list = cmd_list->next;
			cmd_list->next = NULL;
		}
		else
			add_cmd(cmd_list, tmp);
		if (tmp->next == NULL)
			break ;
		tmp = tmp->next;
	}
	handle_params(&head);
	return (head);
}
