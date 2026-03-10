/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/10 16:41:57 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

/*
** Waits for heredoc child, handles SIGINT cancellation,
** reads pipe content on success. Returns NULL on any error/signal.
*/
static char	*heredoc_parent(int pipefd[2], pid_t pid)
{
	int		status;
	char	*content;

	signal(SIGINT, SIG_IGN);
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	termios_change(1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_sigint_received = 1;
		return (close(pipefd[0]), NULL);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return (close(pipefd[0]), NULL);
	rl_on_new_line();
	rl_replace_line("", 0);
	content = read_pipe_content(pipefd[0]);
	close(pipefd[0]);
	return (content);
}

// Forks a heredoc child, returns heap string of all input or NULL on error.
static char	*heredoc_readline(const char *delim, int no_exp, t_shell *mini)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (NULL);
	termios_change(0);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), termios_change(1), NULL);
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(delim, no_exp, mini, pipefd[1]);
	}
	close(pipefd[1]);
	return (heredoc_parent(pipefd, pid));
}

// Runs heredoc_readline for one redirect
// stores result in redir->heredoc_content.
static int	collect_one_heredoc(t_redirect *redir, t_shell *mini)
{
	char	*content;

	g_sigint_received = 0;
	content = heredoc_readline(
			redir->file_token->value,
			redir->file_token->heredoc_no_expand,
			mini);
	if (!content)
	{
		if (g_sigint_received)
			return (130);
		return (1);
	}
	free(redir->heredoc_content);
	redir->heredoc_content = content;
	return (0);
}

/*
** Iterates all commands and their redirects, collecting heredoc content
** for each T_HEREDOC. Sets g_exit_status and returns 1 on any error.
*/
int	collect_heredocs(t_shell *mini)
{
	t_command	*cmd;
	t_redirect	*redir;
	int			status;

	cmd = mini->commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->redir_token->type == T_HEREDOC)
			{
				status = collect_one_heredoc(redir, mini);
				if (status != 0)
					return (mini->g_exit_status = status, 1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
