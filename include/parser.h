/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 17:05:48 by phofer            #+#    #+#             */
/*   Updated: 2026/02/09 17:31:13 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "tokens.h"

int		tokenize_input(t_shell *mini, const char *input);
t_token	*new_token(t_token_type type, const char *str, size_t len);
void	add_token(t_token **head, t_token *new);
int		tokenize_word(t_token **tokens, const char *str, size_t *i);

//utils
void	skip_whitespaces(const char *str, size_t *i);
int		is_operator(char c);
int		is_quote(char c);
void	skip_whitespaces(const char *str, size_t *i);

#endif
