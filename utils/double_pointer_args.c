/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_pointer_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 09:50:29 by hnait             #+#    #+#             */
/*   Updated: 2023/07/14 18:32:57 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_params	double_pointer_to_args(char **double_pointer, t_data *data)
{
	t_params	params;
	int			i;

	i = 0;
	params = NULL;
	while (double_pointer[i])
	{
		if (add_param(&params, double_pointer[i], data) == NULL)
			return (NULL);
		i++;
	}
	return (params);
}