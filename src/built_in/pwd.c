/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:39 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/06 15:05:07 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void pwd(void)
{
    char *cwd;
    
    cwd = malloc(1024 * sizeof(char));
    if (!cwd)
    {
        perror("malloc failed!");
        return;
    }
    if (getcwd(cwd, 1024) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd error!");
    }
    free(cwd);
}
