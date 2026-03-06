/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/06 16:47:38 by phofer           ###   ########.fr       */
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
    if (command[i] && ft_strcmp(command[i], "-n") == 0)//shouldn't only work with -n but also -n -n... or -nnnnn
    {
        flag = 1;
        i++;
    }
    if(!command[1])
    {
        if (flag == 0)
            ft_putstr_fd("\n", STDOUT_FILENO);
        return 0;
    }
    while (command[i])
    {
        ft_putstr_fd(command[i], STDOUT_FILENO);
        if (command[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }
    if (flag == 0)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}
// my approach, should double check when parsing is ready
