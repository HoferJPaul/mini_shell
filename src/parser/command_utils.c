/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/06 16:19:17 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/command.h"

/*
** Initializes a new command structure
**
** @return Newly allocated command, or NULL on malloc failure
*/
t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
** Initializes a new redirect structure
** Just stores pointers to existing tokens - no data duplication
**
** @param redir_token The redirect operator token (<, >, >>, <<)
** @param file_token  The filename/delimiter token
** @return            Newly allocated redirect, or NULL on malloc failure
*/
t_redirect	*init_redirect(t_token *redir_token, t_token *file_token)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->redir_token = redir_token;
	redir->file_token = file_token;
	redir->heredoc_content = NULL;
	redir->next = NULL;
	return (redir);
}

/*
** Frees a list of redirections
** Note: Does NOT free the tokens themselves (they're freed with token list)
**
** @param redir Head of redirect list
*/
void	free_redirects(t_redirect *redir)
{
	t_redirect	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		free(temp->heredoc_content);
		free(temp);
	}
}

/*
** Frees an array of strings (NULL-terminated)
**
** @param arr Array to free
*/
static void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
** Frees a list of commands
**
** @param cmd Head of command list
*/
void	free_commands(t_command *cmd)
{
	t_command	*temp;

	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		free_string_array(temp->args);
		free_redirects(temp->redirects);
		free(temp);
	}
}
void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}
