/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:09:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/09 14:46:49 by phofer           ###   ########.fr       */
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
//master cleanup
void free_dobby(t_shell *mini)
{
    rl_clear_history();
    if(!mini)
    {
        printf("no mini struct initialized!\n");
        return;
    }
    free(mini->cwd);
    free_array(mini->paths);
    free_env_nodes(mini->env);
    free_commands(mini->commands);
    free_tokens(mini->tokens);
}
