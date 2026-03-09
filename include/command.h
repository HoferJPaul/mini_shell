/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/09 14:48:09 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "tokens.h"
# include "minishell.h"

typedef struct s_shell	t_shell;
typedef struct s_token	t_token;

/*
** Redirection structure
** Stores reference to the redirect token and its target file token
** No need to duplicate data - just point to existing tokens
*/
typedef struct s_redirect
{
	t_token				*redir_token;	// The redirect operator token (<, >, >>, <<)
	t_token				*file_token;	// The filename/delimiter token
	char				*heredoc_content;	// Content for heredoc (filled during execution)
	struct s_redirect	*next;
}	t_redirect;

/*
** Command structure
** Represents a single command with its arguments and redirections
*/
typedef struct s_command
{
	char				**args;		// NULL-terminated array: ["echo", "hello", NULL]
	t_redirect			*redirects;	// Linked list of redirections
	struct s_command	*next;		// Next command in pipeline
}	t_command;

int			parse(t_shell *mini);
void		free_commands(t_command *cmd);
void		free_redirects(t_redirect *redir);
void		free_tokens(t_token *tokens);
t_command	*parse_pipeline(t_token *tokens);
t_command	*init_command(void);
t_redirect	*init_redirect(t_token *redir_token, t_token *file_token);
char		**build_args_array(t_token *tokens);
t_redirect	*parse_redirects(t_token *tokens);

#endif
