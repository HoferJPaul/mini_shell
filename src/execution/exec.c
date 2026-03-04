/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/04 15:50:59 by phofer           ###   ########.fr       */
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
** 1. Guard against NULL command list.
**
** 2. If there is more than one command, delegate to execution_pipe()
**    which handles the full pipeline with forked children.
**
** 3. Apply any file redirections for the single command BEFORE running it.
**
** 4. If the command is a builtin that must run in the parent
**    (cd, export, exit, …), execute it directly so state changes persist.
**
** 5. Otherwise fork a child:
**    - Child applies redirections then calls exec_external().
**    - Parent waits and translates the raw waitpid status into $?.
**
** The final result is stored in mini->g_exit_status ($?).
*/
int	execution(t_shell *mini)
{
	pid_t	p;
	int		status;

	if (!mini->commands || !mini->commands->args || !mini->commands->args[0])
		return (1);
	if (mini->commands->next)
		return (execution_pipe(mini));
	/* single command: builtins run in parent to keep state changes.
	** Save stdin/stdout first so redirections don't permanently alter
	** the shell's own file descriptors after the builtin returns. */
	if (check_builtin(mini->commands->args[0]) == 0)
	{
		int	saved_in;
		int	saved_out;
		int	ret;

		saved_in = dup(STDIN_FILENO);
		saved_out = dup(STDOUT_FILENO);
		if (apply_redirections(mini->commands) == -1)
		{
			close(saved_in);
			close(saved_out);
			return (mini->g_exit_status = 1, 1);
		}
		ret = exec_builtin(mini, mini->commands->args);
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return (ret);
	}
	p = fork();
	if (p < 0)
		return (perror("fork"), 1);
	if (p == 0)
	{
		if (apply_redirections(mini->commands) == -1)
			exit(1);
		exec_external(mini, mini->commands->args);
		exit(1); /* backup – exec_external already exits on failure */
	}
	waitpid(p, &status, 0);
	if (WIFEXITED(status))
		mini->g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->g_exit_status = 128 + WTERMSIG(status);
	return (0);
}
