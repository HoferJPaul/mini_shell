/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 19:19:21 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

int echo(char **command)
{
    int flag;
    int i;

    i = 1;
    flag = 0;
    if (command[i] && ft_strcmp(command[i], "-n") == 0)
    {
        flag = 1;
        i++;
    }
    if(!command[1])
    {
        if(flag == 1)
            printf("");
        else
            printf("\n");
        return 0;
    }
    while (command[i+1])
    {
        printf("%s", command[i]);
        if (command[i+1])
            printf(" ");
        i++;
    }
    printf("%s", command[i]);
    if (flag == 0)
        printf("\n");
    else
        printf("%%");
    return 0;
}
// my approach, should double check when parsing is ready
