/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:47:24 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/10 16:48:51 by zgahrama         ###   ########.fr       */
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
    if (path_value && !mini->paths)
        return;
}

void refresh_paths(t_shell *mini) // only called in unset and export
{
    char *path_value;

    if (mini->paths)
        free_array(mini->paths); // destroy old before creating new, leak protection **function needs to be created in cleanup
    path_value = env_get(mini->env, "PATH");
    if (path_value)
        mini->paths = ft_split(path_value, ':');
    else
        mini->paths = NULL;
}
char *path_append(char *old, char *add)
{
    char *tmp;
    char *res;

    if (!old)
        return strdup(add);
    tmp = malloc(strlen(old) + strlen(add) + 2); // to be freed in exit
    sprintf(tmp, "%s:%s", old, add);
    res = tmp;
    return res;
}

void add_new_paths(t_shell *mini, char *new_path)
{
    char *path_value;
    char *tmp;
    path_value = env_get(mini->env, "PATH");

    if (path_value)
        mini->paths = ft_split(path_value, ':');
    else
        mini->paths = NULL;
    tmp = path_append(path_value, new_path);
    env_set(&mini->env, "PATH", tmp);
    free(tmp);
}