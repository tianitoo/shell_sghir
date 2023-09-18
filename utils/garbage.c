/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   garbage.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: kmouradi <kmouradi@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/28 16:47:23 by hnait			 #+#	#+#			 */
/*   Updated: 2023/09/15 15:53:16 by kmouradi		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

t_garbage	*allocate_garbage(void *garbage, t_data *data)
{
	t_garbage	*new;

	new = (t_garbage *)malloc(sizeof(t_garbage));
	if (new == NULL)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);
	}
	new->ptr = garbage;
	new->next = NULL;
	return (new);
}

t_garbage	*add_garbage(t_data *data, void *garbage)
{
	t_garbage	*new;

	new = NULL;
	new = g_exit->garbage;
	if (new == NULL)
	{
		new = allocate_garbage(garbage, data);
		if (new == NULL)
			return (NULL);
		g_exit->garbage = new;
	}
	else
	{
		while (new->next)
			new = new->next;
		new->next = allocate_garbage(garbage, data);
		if (new->next == NULL)
			return (NULL);
	}
	return (new);
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
