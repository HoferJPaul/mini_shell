/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:43:23 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/06 17:20:41 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env *create_env(char *key, char *value)//create the linked list to put the env inside.
{
    t_env *node; 
    node = malloc(sizeof(t_env));//needs to be freed at cleanup!!!
    if (!node)
        return NULL;
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    node->next = NULL;
    return node;
}

t_env *env_from_string(char *str)//parsing the key-value pairs and putting them in the copy env node
{
    char *eq;
    
    eq = ft_strchr(str, '=');
    if (!eq)
        return env_new(str, NULL);

    char *key = ft_strndup(str, eq - str);
    char *value = ft_strdup(eq + 1);

    t_env *node = env_new(key, value);

    free(key);
    free(value);
    return node;
}

t_env *env_copy(char **envp)//copying the env
{
    t_env *head = NULL;
    t_env *tail = NULL;
    t_env *node;
    int i = 0;

    while (envp[i])
    {
        node = env_from_string(envp[i]);

        if (!head)
            head = node;
        else
            tail->next = node;

        tail = node;
        i++;
    }
    return head;
}
