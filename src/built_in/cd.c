/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:22 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/11 16:40:13 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *cd_resolve_target(t_shell *mini, char **argv)
{
    char *target;

    if (argv[2] && argv[3])
    {
        printf("cd: too many arguments\n");
        return NULL;
    }
    if (!argv[2])
    {
        target = env_get(mini->env, "HOME");
        if (!target)
        {
            printf("cd: HOME not set\n");
            return NULL;
        }
        return target;
    }
    else if (ft_strcmp(argv[2], "-") == 0)
    {
        target = env_get(mini->env, "OLDPWD");
        if (!target)
        {
            printf("cd: OLDPWD not set\n");
            return NULL;
        }
        return(target);
    }
    return (argv[2]);
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
    env_set(&mini->env, "OLDPWD", oldpwd);
    env_set(&mini->env, "PWD", new_pwd);
    free(mini->cwd);
    mini->cwd = ft_strdup(new_pwd);
    free(new_pwd);
    free(oldpwd);
}

int cd(t_shell *mini, char **argv)
{
    char	*target;
    char	*old_pwd;

    target = cd_resolve_target(mini, argv);
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
