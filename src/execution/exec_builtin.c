/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 14:01:13 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 17:11:47 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int run_builtin(t_shell *mini)
{
	t_token *curr = mini->tokens;

	if (ft_strcmp(curr->value, "cd") == 0)
		return (cd(mini, curr->next));
	else if (ft_strcmp(curr->value, "export") == 0)
		return (export(&mini->env, curr->next));
	else if (ft_strcmp(curr->value, "unset") == 0)
		return (unset(&mini->env, curr->next));
	else if (ft_strcmp(curr->value, "exit") == 0)
		return (ft_exit(mini, curr->next));
	else if (ft_strcmp(curr->value, "env") == 0)
		return (env(mini->env));
	else if (ft_strcmp(curr->value, "pwd") == 0)
		return (pwd(mini));
	else if (ft_strcmp(curr->value, "echo") == 0)
		return (echo(curr->next));

	return (1);
}
/* Execute builtins directly in parent when state changes must persist. */
int exec_builtin_parent(t_shell *mini)
{
	mini->g_exit_status = run_builtin(mini);
	return (mini->g_exit_status);
}
/* Execute builtins in a forked child (e.g. pipeline) and propagate status. */
int exec_builtin_child(t_shell *mini)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		exit(run_builtin(mini));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))//Lower 7 bits → signal number: If those bits are 0 → normal exit: Upper 8 bits → exit code
		mini->g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->g_exit_status = 128 + WTERMSIG(status);
	return (mini->g_exit_status);
}
