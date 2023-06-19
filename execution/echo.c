/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:38:55 by msekhsou          #+#    #+#             */
/*   Updated: 2023/05/13 18:38:13 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_echo(char **args)
{
    int i;
    int newline;
    int is_notn;
    int j;

    is_notn = 0;
    newline = 0;
    i = 1;
    j = 1;
    if(args[i] != NULL && strncmp(args[i], "-n",2) == 0)
    {
        while (args[i][j] == 'n')
        {
            newline++;
            j++;
        }
        if(args[i][j] != '\0')
            is_notn = 1;
    }
    if(args[i] != NULL && newline > 0 && is_notn == 0)
        i++;
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline == 0)
        printf("\n");
    return (0);
}

// int main(int ac, char **av)
// {
//     // printf("%s\n",av[1]);
//     ft_echo(av);
// }