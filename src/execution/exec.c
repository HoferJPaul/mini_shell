/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/03 15:52:48 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

int	check_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (1);
}

/*
** Executes one command.
**
** 1. If the command is a builtin that must run in the parent
**    (like cd, export, exit), execute it directly.
**
** 2. Otherwise fork a child process:
**    - Child executes the external command with execve().
**    - Parent waits for the child using waitpid().
**
** 3. After waitpid(), the raw status is translated into the
**    shell exit status:
**       - Normal exit  -> exit code
**       - Signal death -> 128 + signal number
**
** The final result is stored in mini->g_exit_status ($?).
*/
int	execution(t_shell *mini)
{
	pid_t	p;
	int		status;

	if (mini->commands && mini->commands->next)
	{
		printf("there is pipe!\n");
		return (execution_pipe(mini));
	} // there is pipe
	if (check_builtin(mini->commands->args[0]) == 0)
		return (exec_builtin(mini, mini->commands->args));
	p = fork();
	if (p < 0)
	{
		perror("fork");
		return (1);
	}
	if (p == 0)
	{
		exec_external(mini, mini->commands->args);
		exit(1); // backup exit
	}
	waitpid(p, &status, 0);
	if (WIFEXITED(status))
		mini->g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->g_exit_status = 128 + WTERMSIG(status);
	return (0);
}
