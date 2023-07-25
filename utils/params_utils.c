#include "../minishell.h"

void	show_params(t_params *params)
{
	t_params	tmp;

	tmp = *params;
	while (tmp->prev)
	{
		tmp = tmp->prev;
	}
	while (tmp)
	{
		ft_printf("|%s| -> ", tmp->parameter);
		tmp = tmp->next;
	}
	ft_printf("\n");
}

t_params	delete_param(t_params *params)
{
	t_params	tmp;

	tmp = *params;

	// show_params(params);*
	ft_printf("showing params before delete\n");
	show_params(params);
	if ((*params)->prev == NULL)
	{
		if ((*params)->next != NULL)
		{
			(*params)->next->prev = NULL;
			*params = (*params)->next;
		}
		else
			*params = NULL;
	}
	else
	{
		if ((*params)->next != NULL)
		{
			(*params)->prev->next = (*params)->next;
			(*params)->next->prev = (*params)->prev;
			*params = (*params)->next;
		}
		else
			(*params)->prev->next = NULL;
	}
	ft_printf("showing params after delete\n");
	show_params(params);
	ft_printf("parameter |%s|\n", (*params)->parameter);
	free(tmp->parameter);
	free(tmp);
	return (*params);
}