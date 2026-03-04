/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/04 15:43:12 by phofer           ###   ########.fr       */
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

/*
** Applies every redirection in the t_redirect list attached to cmd.
** Returns 0 on success, -1 on any error.
** Must be called inside the child process (or parent for builtins).
*/
int	apply_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->redir_token->type == T_REDIR_IN)
		{
			fd = open(redir->file_token->value, O_RDONLY);
			if (fd == -1)
				return (dprintf(STDERR_FILENO, "%s: %s\n",
						redir->file_token->value, strerror(errno)), -1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->redir_token->type == T_REDIR_OUT)
		{
			fd = open(redir->file_token->value,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (dprintf(STDERR_FILENO, "%s: %s\n",
						redir->file_token->value, strerror(errno)), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->redir_token->type == T_APPEND)
		{
			fd = open(redir->file_token->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (dprintf(STDERR_FILENO, "%s: %s\n",
						redir->file_token->value, strerror(errno)), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->redir_token->type == T_HEREDOC)
		{
			if (apply_heredoc(redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
