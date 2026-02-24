/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/24 18:48:22 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int check_tokens(t_token *tokens)
{
    t_token *curr;

    curr = tokens;
    if (ft_strncmp(curr->value, "echo", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        echo(curr);//" character doesn't get removed in tokenization.
        return 0;
    }
    return 1;
}
int execution(t_shell *mini)
{
    if (check_tokens(mini->tokens) == 1)
        printf("gibberish\n");
    return 0;
}
