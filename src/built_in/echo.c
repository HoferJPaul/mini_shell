/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/05 17:03:48 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//should be with -n option, there should be a flag checker in parser
void echo(char *str, int flag)
{
    if(flag == 0)//no -n flag
    {
        printf("%s\n", str);
    }
    else//-n flag caught
    {
        printf("%s", str);
    }
}
//my approach, should double check when parsing is ready
