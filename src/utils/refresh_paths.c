/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:47:24 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/11 12:11:04 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void init_paths(t_env *env, t_shell *mini) // to initialize the path at first
{
    char *path_value;

    path_value = env_get(env, "PATH");

    if (path_value)
        mini->paths = ft_split(path_value, ':');
    else
        mini->paths = NULL;
}

void refresh_paths(t_shell *mini) // only called in unset and export
{
    char *path_value;

    if (mini->paths)
        free_array(mini->paths); // destroy old before creating new, leak protection
    path_value = env_get(mini->env, "PATH");
    if (path_value)
        mini->paths = ft_split(path_value, ':');
    else
        mini->paths = NULL;
}
char *path_append(char *old, char *add)//helper to add a new path to existing one
{
    char *tmp;
    char *res;

    if (!old)
        return ft_strdup(add);
    tmp = ft_strjoin(old, ":");
    if (!tmp)
        return NULL;
    res = ft_strjoin(tmp, add);
    free(tmp);
    return res;
}

void add_new_paths(t_shell *mini, char *new_path)//value of new_path should be recieved in right format
{
    char *old;
    char *new_value;

    old = env_get(mini->env, "PATH");
    new_value = path_append(old, new_path);
    if (!new_value)
        return;
    env_set(&mini->env, "PATH", new_value);
    free(new_value);
    refresh_paths(mini);   // ← rebuild cache
}
