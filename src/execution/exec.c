/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/26 17:54:35 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int check_builtins(t_shell *mini)
{
    t_token *curr;

    curr = mini->tokens;
    if (ft_strncmp(curr->value, "echo", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        echo(curr);//" character doesn't get removed in tokenization.
        return 0;
    }
    else if(ft_strncmp(curr->value, "env", ft_strlen(curr->value)) == 0)
    {
        env(mini->env);
        return 0;
    }
    else if(ft_strncmp(curr->value, "pwd", ft_strlen(curr->value)) == 0)
    {
        pwd(mini);
        return 0;
    }
    else if(ft_strncmp(curr->value, "cd", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        cd(mini, curr);
        return 0;
    }
    else if(ft_strncmp(curr->value, "export", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        export(&mini->env, curr);
        return 0;
    }
    else if(ft_strncmp(curr->value, "unset", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        unset(&mini->env, curr);
        return 0;
    }
    else if(ft_strncmp(curr->value, "exit", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        ft_exit(mini, curr);
        return 0;
    }
    return 1;
}

int execution(t_shell *mini)
{
    if (check_builtins(mini) == 1)
        printf("gibberish\n");
    //needs checking of tokens(do they pipe, is there multiple commands without pipe etc)
    exec_external(mini, mini->tokens);
    
    return 0;
}
