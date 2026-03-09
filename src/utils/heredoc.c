/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/09 18:41:28 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"
#include <termios.h>
//#include <asm-generic/termbits.h>

char	*expand_word(const char *s, t_shell *mini);

// Suppress or restore terminal echoing of control characters (e.g. ^C).
void	termios_change(int echo_ctl_chr)
{
	struct termios	t;

	if (tcgetattr(STDOUT_FILENO, &t) == -1)
		return ;
	if (echo_ctl_chr)
		t.c_lflag |= ECHOCTL;
	else
		t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &t);
}

static char	*read_pipe_content(int fd)
{
	char	buf[4097];
	char	*content;
	char	*tmp;
	ssize_t	n;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	n = read(fd, buf, 4096);
	while (n > 0)
	{
		buf[n] = '\0';
		tmp = ft_strjoin(content, buf);
		free(content);
		if (!tmp)
			return (NULL);
		content = tmp;
		n = read(fd, buf, 4096);
	}
	if (n < 0)
		return (free(content), NULL);
	return (content);
}

static void	sigint_exit(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_DFL);
	kill(getpid(), SIGINT);
}

static int	write_processed_line(int write_fd, char *line,
		t_shell *mini, int no_exp)
{
	char	*expanded;

	if (no_exp)
		expanded = ft_strdup(line);
	else
		expanded = expand_word(line, mini);
	if (!expanded)
		return (-1);
	if (write(write_fd, expanded, ft_strlen(expanded)) == -1)
		return (free(expanded), -1);
	if (write(write_fd, "\n", 1) == -1)
		return (free(expanded), -1);
	free(expanded);
	return (0);
}

static void	heredoc_child(const char *delim, int no_exp,
		t_shell *mini, int write_fd)
{
	char	*line;

	signal(SIGINT, sigint_exit);
	signal(SIGQUIT, SIG_DFL);
	line = readline("heredoc> ");
	while (line && ft_strcmp(line, delim) != 0)
	{
		if (write_processed_line(write_fd, line, mini, no_exp) == -1)
		{
			free(line);
			close(write_fd);
			_exit(1);
		}
		free(line);
		line = readline("heredoc> ");
	}
	free(line);
	close(write_fd);
	_exit(0);
}

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
		close(pipefd[0]);
		return (NULL);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (NULL);
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	content = read_pipe_content(pipefd[0]);
	close(pipefd[0]);
	return (content);
}

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

static int	collect_cmd_heredocs(t_command *cmd, t_shell *mini)
{
	t_redirect	*redir;
	int			status;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->redir_token->type == T_HEREDOC)
		{
			status = collect_one_heredoc(redir, mini);
			if (status != 0)
				return (status);
		}
		redir = redir->next;
	}
	return (0);
}

int	collect_heredocs(t_shell *mini)
{
	t_command	*cmd;
	int			status;

	cmd = mini->commands;
	while (cmd)
	{
		status = collect_cmd_heredocs(cmd, mini);
		if (status != 0)
		{
			mini->g_exit_status = status;
			return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
