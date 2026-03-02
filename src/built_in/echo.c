/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 15:18:31 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

int echo(t_token *tokens)
{
    t_token *curr;
    int flag;

    curr = tokens;
    flag = 0;
    printf("inside echo function\n");
    if (ft_strncmp(curr->value, "-n", ft_strlen(curr->value)) == 0)
    {
        flag = 1;
        curr = curr->next;
    }
    while (curr->next)
    {
        printf("%s", curr->value);
        if (curr->next)
            printf(" ");
        curr = curr->next;
    }
    printf("%s", curr->value);
    if (flag == 0)
        printf("\n");
    else
        printf("%%");
    return 0;
}
// my approach, should double check when parsing is ready
