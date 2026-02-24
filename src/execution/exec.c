/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/24 14:50:04 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"
static int lst_to_str(t_token *tokens, char *str)
{
    t_token *curr;
    int flag;

    curr = tokens;
    flag = 0;
    if(ft_strncmp(curr->value, "-n", ft_strlen(curr->value)) == 0)
        {
            flag = 1;
            curr = curr->next;
        }
        str = curr->value;
        str = ft_strjoin(str, " ");
        while (curr->next)
        {
            str = ft_strjoin(str, curr->next->value);
            if(curr->next->next)
                str = ft_strjoin(str, " ");
            curr = curr->next;
        }
    return flag;
}
static int check_tokens(t_token *tokens)
{
    t_token *curr;
    int flag;
    char *str;

    str = malloc(1024);
    curr = tokens;
    if (ft_strncmp(curr->value, "echo", ft_strlen(curr->value)) == 0)
    {
        curr = curr->next;
        flag = lst_to_str(curr, str);
        echo(str, flag);
        free(str);
        return 0;
    }
    free(str);
    return 1;
}
int execution(t_shell *mini)
{
    if (check_tokens(mini->tokens) == 1)
        printf("gibberish\n");
    return 0;
}
