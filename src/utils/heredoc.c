/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/04 18:16:30 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

char	*expand_word(const char *s, t_shell *mini);

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

static char	*heredoc_readline(const char *delim, int no_exp, t_shell *mini)
{
	char	*content;
	char	*line;

	signal(SIGINT, heredoc_sigint);
	content = ft_strdup("");
	if (!content)
		return (NULL);
	line = readline("> ");
	while (line && ft_strcmp(line, delim) != 0)
	{
		content = append_line(content, line, mini, no_exp);
		free(line);
		if (!content)
			return (NULL);
		line = readline("> ");
	}
	free(line);
	signal(SIGINT, sigint_handler);
	if (g_sigint_received)
		return (free(content), NULL);
	return (content);
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
