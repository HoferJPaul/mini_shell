/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/09 14:37:24 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

char	*expand_word(const char *s, t_shell *mini);
void	termios_change(int echo_ctl_chr);

static char	*append_line(char *content, char *line, t_shell *mini, int no_exp);

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
	return (content);
}

static void	sigint_exit(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_DFL);
	kill(getpid(), SIGINT);
}

static void	heredoc_child(const char *delim, int no_exp, t_shell *mini,
		int write_fd)
{
	char	*content;
	char	*line;

	signal(SIGINT, sigint_exit);
	signal(SIGQUIT, SIG_DFL);
	content = ft_strdup("");
	if (!content)
		_exit(1);
	line = readline("heredoc> ");
	while (line && ft_strcmp(line, delim) != 0)
	{
		content = append_line(content, line, mini, no_exp);
		free(line);
		if (!content)
			_exit(1);
		line = readline("heredoc> ");
	}
	free(line);
	write(write_fd, content, ft_strlen(content));
	free(content);
	_exit(0);
}

static char	*heredoc_parent(int pipefd[2], pid_t pid)
{
	int		status;
	char	*content;

	signal(SIGINT, SIG_IGN);
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	signal(SIGINT, sigint_handler);
	termios_change(1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_redisplay();
		g_sigint_received = 1;
		close(pipefd[0]);
		return (NULL);
	}
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

static char	*append_line(char *content, char *line, t_shell *mini, int no_exp)
{
	char	*expanded;
	char	*tmp;

	if (!no_exp)
		expanded = expand_word(line, mini);
	else
		expanded = ft_strdup(line);
	if (!expanded)
		return (free(content), NULL);
	tmp = ft_strjoin(content, expanded);
	free(expanded);
	free(content);
	if (!tmp)
		return (NULL);
	content = tmp;
	tmp = ft_strjoin(content, "\n");
	free(content);
	return (tmp);
}

static int	collect_one_heredoc(t_redirect *redir, t_shell *mini)
{
	char	*content;

	content = heredoc_readline(
			redir->file_token->value,
			redir->file_token->heredoc_no_expand,
			mini);
	if (!content && g_sigint_received)
		return (-1);
	if (!content)
		return (-1);
	free(redir->heredoc_content);
	redir->heredoc_content = content;
	return (0);
}

static int	collect_cmd_heredocs(t_command *cmd, t_shell *mini)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->redir_token->type == T_HEREDOC)
			if (collect_one_heredoc(redir, mini) == -1)
				return (-1);
		redir = redir->next;
	}
	return (0);
}

int	collect_heredocs(t_shell *mini)
{
	t_command	*cmd;

	cmd = mini->commands;
	while (cmd)
	{
		if (collect_cmd_heredocs(cmd, mini) == -1)
			return (mini->g_exit_status = 130, 1);
		cmd = cmd->next;
	}
	return (0);
}
