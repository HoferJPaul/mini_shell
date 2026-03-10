/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:22:37 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

/*
** Heredoc content was already collected during parsing and stored in
** redir->heredoc_content. We just pipe it into stdin here at exec time.
*/
static int	apply_heredoc(t_redirect *redir)
{
	int		fd[2];

	if (!redir->heredoc_content)
		return (0);
	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	ft_putstr_fd(redir->heredoc_content, fd[1]);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	return (0);
}

// Prints strerror for file, returns -1.
static int	redir_error(char *file)
{
	dprintf(STDERR_FILENO, "%s: %s\n", file, strerror(errno));
	return (-1);
}

// Opens file with given flags, dup2s to target_fd; returns -1 on error.
static int	redir_open(char *file, int flags, int target_fd)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
		return (redir_error(file));
	dup2(fd, target_fd);
	close(fd);
	return (0);
}

// Dispatches one redirection by type; returns -1 on error.
static int	apply_one_redir(t_redirect *redir)
{
	int	type;

	type = redir->redir_token->type;
	if (type == T_REDIR_IN)
		return (redir_open(redir->file_token->value,
				O_RDONLY, STDIN_FILENO));
	if (type == T_REDIR_OUT)
		return (redir_open(redir->file_token->value,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO));
	if (type == T_APPEND)
		return (redir_open(redir->file_token->value,
				O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO));
	if (type == T_HEREDOC)
		return (apply_heredoc(redir));
	return (0);
}

// Applies all redirections in cmd->redirects chain; returns -1 on first error.
int	apply_redirections(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (apply_one_redir(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
