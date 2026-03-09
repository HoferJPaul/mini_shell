/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/09 18:43:55 by phofer           ###   ########.fr       */
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
*/
typedef struct s_redirect
{
	t_token				*redir_token;
	t_token				*file_token;
	char				*heredoc_content;
	struct s_redirect	*next;
}	t_redirect;

/*
** Command structure
** Represents a single command with its arguments and redirections
*/
typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
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
