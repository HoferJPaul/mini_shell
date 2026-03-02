/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:22 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 18:33:56 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static char *cd_resolve_target(t_shell *mini, char **command)
{
    char *target;

    if (!command[1])
    {
        target = env_get(mini->env, "HOME");
        if (!target)
        {
            printf("cd: HOME not set\n");
            return NULL;
        }
        return target;
    }
    if (command[2])
    {
        printf("cd: too many arguments\n");
        return NULL;
    }
    else if (ft_strcmp(command[2], "-") == 0)
    {
        target = env_get(mini->env, "OLDPWD");
        if (!target)
        {
            printf("cd: OLDPWD not set\n");
            return NULL;
        }
        return(target);
    }
    return (command[1]);
}

static int cd_change_directory(char *target)
{
    if (chdir(target) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}

static void cd_update_pwd_vars(t_shell *mini, char *oldpwd)
{
    char *new_pwd;

    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("cd");
        return;
    }
    env_set(&mini->env, "OLDPWD", oldpwd, 0);
    env_set(&mini->env, "PWD", new_pwd, 0);
    free(mini->cwd);
    mini->cwd = ft_strdup(new_pwd);
    free(new_pwd);
    free(oldpwd);
}
/*
** cd builtin:
** 1) (helper)Resolve target: HOME if no arg, OLDPWD if "-", else argv[2].
** 2) Save current directory with getcwd().
** 3) chdir() to target; on failure, print error and keep cwd unchanged.
** 4) On success, update OLDPWD/PWD in env and mini->cwd.
*/
int cd(t_shell *mini, char **command)
{
    char	*target;
    char	*old_pwd;

    target = cd_resolve_target(mini, command);
    if (!target)
        return (mini->g_exit_status = 1);
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        perror("cd: getcwd");
        return (mini->g_exit_status = 1);
    }
    if (cd_change_directory(target) != 0)
    {
        free(old_pwd);
        return (mini->g_exit_status = 1);
    }
    cd_update_pwd_vars(mini, old_pwd);
    return (mini->g_exit_status = 0);
}
