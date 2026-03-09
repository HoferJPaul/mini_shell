/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 14:01:13 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/09 18:51:59 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int	run_builtin(t_shell *mini, char **command)
{

	if (ft_strcmp(command[0], "cd") == 0)
		return (cd(mini, command));
	else if (ft_strcmp(command[0], "export") == 0)
		return (export(&mini->env, command));
	else if (ft_strcmp(command[0], "unset") == 0)
		return (unset(&mini->env, command));
	else if (ft_strcmp(command[0], "exit") == 0)
		return (ft_exit(mini, command));
	else if (ft_strcmp(command[0], "env") == 0)
		return (env(mini->env));
	else if (ft_strcmp(command[0], "pwd") == 0)
		return (pwd(mini));
	else if (ft_strcmp(command[0], "echo") == 0)
		return (echo(command));
	return (1);
}

/* Execute builtins directly in parent when state changes must persist. */
int	exec_builtin(t_shell *mini, char **command)
{
	mini->g_exit_status = run_builtin(mini, command);
	return (mini->g_exit_status);
}
