/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/17 16:48:34 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "tokens.h"
# include "minishell.h"

/*
** Redirection structure
** Stores information about input/output redirections for a command
*/

typedef struct s_redirect
{
	t_token_type		type;		// T_REDIR_IN, T_REDIR_OUT, T_APPEND, T_HEREDOC
	char				*file;		// Filename or heredoc delimiter
	char				*heredoc_content;	// Content for heredoc (if applicable)
	int					expand;		// 1 = expand variables in heredoc, 0 = don't
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

/*
** Parser functions
*/
int			parse_main(t_shell *mini);
void		free_commands(t_command *cmd);
void		free_redirects(t_redirect *redir);

#endif
