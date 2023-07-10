/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_pointer_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 09:50:29 by hnait             #+#    #+#             */
/*   Updated: 2023/07/10 09:53:01 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_params	double_pointer_to_args(char **double_pointer)
{
	t_params	params;
	int			i;

	i = 0;
	while (double_pointer[i])
	{
		add_param(&params, double_pointer[i]);
		i++;
	}
	return (params);
}