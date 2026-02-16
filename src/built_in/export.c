/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:14 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/16 13:39:42 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int check_var_format(char *var)
{
    int i;

    if (var[0] == '_' || ft_isalpha(var[0]) == 1)
    {
        i = 1;
        while (var[i] && var[i] != '=')
        {
            if (var[i] == '_' || ft_isalnum(var[i]) == 1)
                i++;
        }
        return 1;
    }
    perror("export: wrong input format");
    return -1;
}
char *parse_key(char *var)
{
    char *key;
    int i;

    if(check_var_format(var) == -1)
        return NULL;
    key = malloc(ft_strlen(var) * sizeof(char)); // to be freed after use.
    if (!key)
        return NULL;
    i = 0;
    while (var[i])
    {
        if (var[i] == '=')
            return key;
        key[i] = var[i];
        i++;
    }
    return key;
}

char *parse_val(char *var)
{
    char *val;
    int i;
    int idx;
    
    val = malloc(ft_strlen(var) * sizeof(char));// to be freed.
    if(!val)
        return NULL;
    i = ft_strlen(var) - ft_strlen(parse_key(var)) + 1;
    while(var[i])
    {
        val[idx] = var[i]; 
        i++;
        idx++;
    }
    return val;
}

int export(t_env *env, char *var)
{
    char *key;
    char *val;
    
    key = parse_key(var);
    val = parse_val(var);
    if(!key)
        return -1;
    if(!val)
    {
        env_set(&env, key, NULL, 1);
        print_env(env);
        return 1;
    }
    env_set(&env, key, val, 1);
    free(key);
    free(val);
    return 1;
}
