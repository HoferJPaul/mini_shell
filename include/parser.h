/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 17:05:48 by phofer            #+#    #+#             */
/*   Updated: 2026/03/06 16:18:01 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "tokens.h"

// ================ Expansion =====================

/*
** Helper for expand_word: processes a single character based on its type.
** Handles quotes, variables, and literal characters.
**
** @param s    Input string
** @param i    Pointer to current index
** @param ctx  Buffer context
** @param mini Shell structure
** @return     1 on success, 0 on malloc failure
*/
int		process_char(const char *s, size_t *i,
			t_buf_ctx *ctx, t_shell *mini);
/*
** Reallocates buffer to double its capacity when full.
** Copies existing content to new buffer.
**
** @param ctx  Buffer context containing buffer, length, and capacity
** @return     1 on success, 0 on malloc failure
*/
int		realloc_buffer(t_buf_ctx *ctx);
/*
** Appends a single character to the dynamic buffer.
** Automatically reallocates buffer if capacity is reached.
**
** @param ctx  Buffer context
** @param c    Character to append
** @return     1 on success, 0 on malloc failure
*/
int		append_char(t_buf_ctx *ctx, char c);
/*
** Appends a string to the dynamic buffer.
** Calls append_char for each character in the string.
**
** @param ctx  Buffer context
** @param str  String to append
** @return     1 on success, 0 on malloc failure
*/
int		append_str(t_buf_ctx *ctx, const char *str);
/*
** Determines the length of a variable name starting at position i.
** Handles special variables: $? (exit status), $0-$9 (positional params).
** Standard variables can contain alphanumeric chars and underscores.
**
** @param s    The input string
** @param i    Starting position (after the $)
** @return     Length of the variable name
*/
size_t	get_var_name_len(const char *s, size_t i);
/*
** Retrieves the value of a special variable ($? or $0-$9).
**
** @param name First character of variable name
** @param mini Shell structure containing exit status
** @return     Newly allocated string with value, or NULL
*/
char	*get_special_var(char name, t_shell *mini);

//============================= Tokenizer ================================

int		tokenize(t_shell *mini, const char *input);
t_token	*new_token(t_token_type type, const char *str, size_t len);
void	add_token(t_token **head, t_token *new);
int		tokenize_word(t_token **tokens, const char *str, size_t *i);
int		expand(t_shell *mini);

//utils
void	skip_whitespaces(const char *str, size_t *i);
int		is_operator(char c);
int		is_quote(char c);
void	skip_whitespaces(const char *str, size_t *i);
void	free_tokens(t_token *tokens);

#endif
