/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 17:29:00 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int	check_builtin(t_shell *mini)
{
	t_token	*curr;

	curr = mini->tokens;
	if (ft_strcmp(curr->value, "cd") == 0)
		return (0);
	else if (ft_strcmp(curr->value, "env") == 0)
		return (0);
	else if (ft_strcmp(curr->value, "pwd") == 0)
		return (0);
	else if (ft_strcmp(curr->value, "echo") == 0)
		return (0); //" character doesn't get removed in tokenization.
	else if (ft_strcmp(curr->value, "export") == 0)
		return (0);
	else if (ft_strcmp(curr->value, "unset") == 0)
		return (0);
	else if (ft_strcmp(curr->value, "exit") == 0)
		return (0);
	return (1);
}
/*
** Checks if command is a single builtin with no pipes
*/
static int	is_single_builtin(t_command *commands)
{
	if (!commands || commands->next)
		return (0);
	if (!commands->args || !commands->args[0])
		return (0);
	return (check_builtin(commands->args[0]));
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

	if (check_builtin(mini) == 0 && mini->has_pipe == 0)
		return (exec_builtin_parent(mini));
	else if(check_builtin(mini) == 0 && mini->has_pipe == 1)
		return(exec_builtin_child(mini));
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

/*
** Main execution entry point
** Handles heredocs, then executes pipeline or single builtin
**
** @param mini Shell structure
** @return     0 on success, 1 on error
*/
int	execute(t_shell *mini)
{
	int	status;

	if (!mini || !mini->commands)
		return (0);
	//todo
	// if (process_heredocs(mini->commands) != 0)
	// 	return (1);
	if (is_single_builtin(mini->commands))
	{
		status = execute_single_builtin(mini, mini->commands);
		mini->g_exit_status = status;
		return (0);
	}
	return (execute_pipeline(mini, mini->commands));
}
