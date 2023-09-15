/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_lstadd_back.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/23 13:22:42 by hnait 	   		   #+#	#+#			 */
/*   Updated: 2023/03/14 17:38:58 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*node;

	if (!new)
		return ;
	if (lst)
	{
		node = *lst;
		if (node)
		{
			while (node->next)
				node = node->next;
			node->next = new;
		}
		else
			*lst = new;
	}
}
