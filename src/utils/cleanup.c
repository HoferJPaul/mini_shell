/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:09:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 14:20:29 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//should free the add_history with clear_history here
void free_array(char **arr)//use it for mini->paths
{
    int i = 0;

    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
void free_env_nodes(t_env *env)
{
    t_env *curr;
    t_env *next;
    
    curr = env;
    while(curr != NULL)
    {
        next = curr->next;
        free(curr->key);
        if(curr->value)
            free(curr->value);
        free(curr);
        curr = next;
    }
}
