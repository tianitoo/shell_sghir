/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 16:47:23 by hnait             #+#    #+#             */
/*   Updated: 2023/07/31 08:30:26 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_garbage(void *garbage)
{
	t_garbage	*new;

	new = NULL;
	new = g_exit->garbage;
	if (new == NULL)
	{
		new = (t_garbage *)malloc(sizeof(t_garbage));
		new->ptr = garbage;
		new->next = NULL;
		g_exit->garbage = new;
	}
	else
	{
		while (new->next)
			new = new->next;
		new->next = (t_garbage *)malloc(sizeof(t_garbage));
		new->next->ptr = garbage;
		new->next->next = NULL;
	}
}

void	free_garbage(void)
{
	t_garbage	*tmp;
	t_garbage	*next;

	tmp = g_exit->garbage;
	
	while (tmp)
	{
		next = tmp->next;
		if (tmp->ptr)
		{
			// ft_printf("freeing: %s\n", tmp->ptr);
			free(tmp->ptr);
			tmp->ptr = NULL;
		}
		if (tmp)
		{	
			free(tmp);
			tmp = NULL;
		}
		tmp = next;
	}
	g_exit->garbage = NULL;
}