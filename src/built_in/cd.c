/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:22 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/06 16:20:45 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int cd(char *path)
{
    char *oldpwd;
    char *home;
    char *newpwd;

    oldpwd = getcwd(NULL, 0);

    if (!path)
    {
        home = getenv("HOME");
        if (!home)
            return (printf("cd: HOME not set\n"), 1);
        path = home;
    }

    if (chdir(path) == -1)
        return (perror("cd error!"), free(oldpwd), 1);

    newpwd = getcwd(NULL, 0);
    setenv("OLDPWD", oldpwd, 1);
    setenv("PWD", newpwd, 1);
    printf("%s\n", newpwd);
    free(oldpwd);
    free(newpwd);
    return (0);
}
