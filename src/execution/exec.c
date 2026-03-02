/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 19:42:29 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int	check_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (0); //" character doesn't get removed in tokenization.
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (1);
}

static int	execution_pipe(t_shell *mini)//start by working on this
{
	t_command	*curr;
	char		*std_output;
	int			*fd;
	pid_t		p1;
	pid_t		p2;

	fd = malloc(2 * sizeof(int)); // free after finished using
	pipe(fd);
	curr = mini->commands;
	while (curr)
	{
		p1 = fork();
		if (p1 == 0)
		{
			dup2(fd[1], STDOUT_FILENO); // stdout → pipe write
			close(fd[0]);
			close(fd[1]);
			if (check_builtin(curr->args[0]) == 0)
				exec_builtin_child(mini, curr);
			else
				exec_external(mini, curr->args);
		}
		p2 = fork();
		if (p2 == 0)
		{
			dup2(fd[0], STDIN_FILENO); // stdin ← pipe read
			close(fd[1]);
			close(fd[0]);
			if (check_builtin(curr->args[0]) == 0)
				exec_builtin_child(mini, curr);
			else
				exec_external(mini, curr->args);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(p1, NULL, 0);
		waitpid(p2, NULL, 0);
		curr = curr->next;
	}
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

	if (mini->commands->redirects != NULL) // there is pipe
		return (execution_pipe(mini));
	if (check_builtin(mini->commands->args[0]) == 0)
		return (exec_builtin_parent(mini));
	p = fork();
	if (p < 0)
	{
		perror("fork");
		return (1);
	}
	if (p == 0)
	{
		exec_external(mini, mini->tokens);
		exit(1); // backup exit
	}
	waitpid(p, &status, 0);
	if (WIFEXITED(status))
		mini->g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->g_exit_status = 128 + WTERMSIG(status);
	return (0);
}
