/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/10 16:47:02 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"
#include <termios.h>

char	*expand_word(const char *s, t_shell *mini);

// Suppresses or restores terminal echoing of control characters (e.g. ^C).
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

// Reads all content from fd into a heap string; returns NULL on error.
char	*read_pipe_content(int fd)
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

// Resets SIGINT to default and re-raises it to propagate to parent.
static void	sigint_exit(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_DFL);
	kill(getpid(), SIGINT);
}

// Expands (or duplicates) line, writes it + newline to write_fd.
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

/*
** Heredoc child process: reads lines with readline until delim is matched,
** writing each processed line to write_fd. Exits 0 on success, 1 on error.
*/
void	heredoc_child(const char *delim, int no_exp,
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
