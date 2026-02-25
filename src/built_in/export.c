/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:14 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/25 16:08:19 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

int check_var_format(char *var)
{
    int i;

    if (var[0] && (var[0] == '_' || ft_isalpha(var[0]) == 1))
    {
        i = 1;
        while (var[i] && var[i] != '=')
        {
            if (var[i] != '_' && ft_isalnum(var[i]) != 1)
            {
                ft_putstr_fd("export: not a valid identifier\n", 2);
                return -1;
            }
            i++;
        }
        return 1;
    }
    ft_putstr_fd("export: not a valid identifier\n", 2);
    return -1;
}
char *parse_key(char *var)
{
    char *key;
    int i;
    int len;

    if (check_var_format(var) == -1)
        return NULL;
    i = 0;
    while (var[i] && var[i] != '=')
        i++;
    len = i;
    key = malloc(sizeof(char) * (len + 1));
    if (!key)
        return NULL;
    i = 0;
    while (i < len)
    {
        key[i] = var[i];
        i++;
    }
    key[i] = '\0';
    return key;
}

char *parse_val(char *var)
{
    char *val;
    int i;
    int idx;
    int val_len;

    i = 0;
    while (var[i] && var[i] != '=')
        i++;
    if (!var[i])
        return NULL;
    i++;
    val_len = ft_strlen(var + i);
    val = malloc(sizeof(char) * (val_len + 1));
    if (!val)
        return NULL;
    idx = 0;
    while (var[i])
    {
        val[idx] = var[i];
        i++;
        idx++;
    }
    val[idx] = '\0';
    return val;
}
static int too_many_arg(t_token *tokens)
{
    t_token *curr;
    
    curr = tokens;
    if(!curr->next)
        return 0;
    printf("export: too many arguments\n");
    return 1;
}

// Set or update an environment variable; if no value is given, mark as exported.
int export(t_env **env, t_token *tokens)
{
    char *key;
    char *val;
    
    if (!tokens || !tokens->value)
    {
        print_env(*env);
        return 1;
    }
    if(too_many_arg(tokens) == 1)
        return 1;//unsure what kind of error i should set here
    key = parse_key(tokens->value);
    val = parse_val(tokens->value);
    if (!val)
    {
        env_set(env, key, NULL, 1);
        free(key);
        free(val);
        return 1;
    }
    env_set(env, key, val, 1);
    free(key);
    free(val);
    return 1;
}
