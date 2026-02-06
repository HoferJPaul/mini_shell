/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:55:19 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/06 12:39:54 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//already tested as a function but need to re-test it when the parser is ready :D
void env(char **envp)
{
    char **new_env = envp;
    
    while (*new_env != 0)
    {
        char *thisEnv = *new_env;
        printf("%s\n", thisEnv);
        new_env++;
    }
}