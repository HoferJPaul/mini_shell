/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:52 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/25 16:14:12 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int validate_var(char *var)
{
     int i;

    if (var[0] && (var[0] == '_' || ft_isalpha(var[0]) == 1))
    {
        i = 1;
        while (var[i])
        {
            if (var[i] != '_' && ft_isalnum(var[i]) != 1)
                return 0;
            i++;
        }
        return 1;
    }
    return 0;
}
static int check_var_exists(t_env **env, char *var)
{
    t_env *curr = *env;
    size_t len = ft_strlen(var);

    while (curr)
    {
        if (ft_strncmp(curr->key, var, len) == 0
            && curr->key[len] == '\0')
            return 1;
        curr = curr->next;
    }
    return 0;
}
static void remove_var(t_env **env, char *var)
{
    t_env *curr;
    t_env *prev;
    t_env *next;
    size_t len;

    curr = *env;
    prev = NULL;
    len = ft_strlen(var);
    while (curr)
    {
        next = curr->next;

        if (ft_strncmp(curr->key, var, len) == 0
            && curr->key[len] == '\0')
        {
            if (prev)
                prev->next = next;
            else
                *env = next;

            free(curr->key);
            free(curr->value);
            free(curr);
        }
        else
            prev = curr;
        curr = next;
    }
}
// Removes one or more environment variables by name.
// Returns 0 on success, 1 on failure (no arguments or invalid identifier).
int unset(t_env **env, t_token *tokens)
{
    int i;
    t_token *curr;
    
    curr = tokens;
    if(!curr)
    {
        ft_putstr_fd("unset: no arguments\n", 2);
        return 1;//fail
    }
    i = 0;
    while(curr)
    {
        if(!validate_var(curr->value))
        {
            ft_putstr_fd("unset: not a valid identifier\n", 2);
            curr = curr->next;
            continue;
        }
        if(!check_var_exists(env, curr->value))
        {
            curr = curr->next;
            continue;
        }
        remove_var(env, curr->value);
        curr = curr->next;
    }
    return 0;//success
}
