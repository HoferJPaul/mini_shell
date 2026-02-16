/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:14 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/16 15:31:18 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

// Set or update an environment variable; if no value is given, mark as exported.
int export(t_env **env, char *var)
{
    char *key;
    char *val;

    key = parse_key(var);
    val = parse_val(var);
    if (!key)
    {
        print_env(env);
        free(key);
        free(val);
        return 1;
    }
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
