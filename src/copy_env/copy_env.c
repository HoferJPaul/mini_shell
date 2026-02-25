/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:43:23 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/25 14:19:32 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env *create_env(char *key, char *value, int exported)//create the linked list node to put the key<->value pairs inside.
{
    t_env *node; 
    node = malloc(sizeof(t_env));//needs to be freed at cleanup!!!
    if (!node)
        return NULL;
    node->key = ft_strdup(key);
    if(!value)
        node->value = NULL;
    else
        node->value = ft_strdup(value);
    node->exported_flag = exported;
    node->next = NULL;
    return node;
}

t_env *env_from_string(char *str)//parsing the key<->value pairs and putting them in the copy env node
{
    char *eq;
    
    eq = ft_strchr(str, '=');
    if (!eq)
        return create_env(str, NULL, 1);

    char *key = strndup(str, eq - str);//need to write ours
    char *value = ft_strdup(eq + 1);

    t_env *node = create_env(key, value, 1);

    free(key);
    free(value);
    return node;
}

t_env *env_copy(char **envp)//copying the envp inside our structure
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
