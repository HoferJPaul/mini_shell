/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:22 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:02:07 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

// Returns HOME env var, or NULL with error.
static char	*cd_get_home(t_shell *mini)
{
	char	*target;

	target = env_get(mini->env, "HOME");
	if (!target)
	{
		printf("cd: HOME not set\n");
		return (NULL);
	}
	return (target);
}

/*
** Resolves the cd target path from command args:
** - No arg      -> $HOME
** - "-"         -> $OLDPWD
** - Too many    -> error, NULL
** - Otherwise   -> command[1]
*/
static char	*cd_resolve_target(t_shell *mini, char **command)
{
	char	*target;

	if (!command[1])
		return (cd_get_home(mini));
	if (command[2])
	{
		ft_putendl_fd("cd: too many arguments\n", STDERR_FILENO);
		return (NULL);
	}
	if (ft_strcmp(command[1], "-") == 0)
	{
		target = env_get(mini->env, "OLDPWD");
		if (!target)
		{
			ft_putendl_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (target);
	}
	return (command[1]);
}

// Calls chdir(); prints error and returns 1 on failure.
static int	cd_change_directory(char *target)
{
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

// Updates OLDPWD, PWD and mini->cwd after a successful chdir().
static int	cd_update_pwd_vars(t_shell *mini, char *oldpwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd");
		return (1);
	}
	env_set(&mini->env, "OLDPWD", oldpwd, 0);
	env_set(&mini->env, "PWD", new_pwd, 0);
	free(mini->cwd);
	mini->cwd = ft_strdup(new_pwd);
	free(new_pwd);
	free(oldpwd);
	return (0);
}

/*
** cd builtin:
** 1) (helper)Resolve target: HOME if no arg, OLDPWD if "-", else argv[2].
** 2) Save current directory with getcwd().
** 3) chdir() to target; on failure, print error and keep cwd unchanged.
** 4) On success, update OLDPWD/PWD in env and mini->cwd.
*/
int	cd(t_shell *mini, char **command)
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
	if (cd_update_pwd_vars(mini, old_pwd) == 1)
	{
		free(old_pwd);
		return (mini->g_exit_status = 1);
	}
	return (mini->g_exit_status = 0);
}
