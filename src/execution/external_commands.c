/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:11:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/26 17:55:15 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokens.h"
#include "../../include/minishell.h"

char *find_command_path(t_shell *mini, char *cmd)
{
    char    *path;
    char    **dirs;
    char    *full_path;
    char    *tmp;
    int     i;

    path = env_get(mini->env, "PATH");
    if (!path)
        return NULL;

    dirs = ft_split(path, ':');
    i = 0;
    while (dirs[i])
    {
        tmp = ft_strjoin(dirs[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
            return full_path;
        free(full_path);
        i++;
    }
    free_array(dirs);
    return NULL;
}

//just executing it as it is, doesn't check if theres a pipe, assumes already being called in the child process
void exec_external(t_shell *mini, t_token *tokens)
{
    char *path;
    t_token *curr;
    
    curr = tokens;
    if (ft_strchr(curr->value, '/'))
        path = curr->value;
    else
        path = find_command_path(mini, curr->value);
    if (!path)
    {
        printf("%s: command not found\n", curr->value);
        exit(127);
    }
    execve(path, curr->value, env_to_array(mini->env));
    perror("execve");
    exit(1);
}

