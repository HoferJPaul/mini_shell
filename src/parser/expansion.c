/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 15:37:41 by phofer            #+#    #+#             */
/*   Updated: 2026/02/16 16:57:39 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/parser.h"

/*
** Removes all quote characters (both ' and ") from a string.
** Used for heredoc delimiters where we need to strip quotes
** but don't care about quote pairing.
**
** @param s    The input string containing quotes
** @return     Newly allocated string without quotes, or NULL on malloc failure
*/
static char	*strip_quotes(const char *s)
{
	char	*out;
	size_t	i;
	size_t	k;

	out = malloc(ft_strlen(s) + 1);
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (s[i])
	{
		if (s[i] != '\'' && s[i] != '"')
			out[k++] = s[i];
		i++;
	}
	out[k] = '\0';
	return (out);
}

/*
** Handles heredoc delimiter token by:
** 1. Checking if delimiter contains quotes
** 2. Setting heredoc_no_expand flag (1 if quoted, 0 if not)
** 3. Stripping all quotes from the delimiter value
**
** @param t    Token representing the heredoc delimiter
** @return     0 on success, 1 on error
*/
int	handle_heredoc(t_token *t)
{
	char	*stripped;

	if (!t || !t->value)
		return (1);
	t->heredoc_no_expand = 0;
	if (ft_strchr(t->value, '\'') || ft_strchr(t->value, '"'))
		t->heredoc_no_expand = 1;
	stripped = strip_quotes(t->value);
	if (!stripped)
		return (1);
	free(t->value);
	t->value = stripped;
	return (0);
}

/*
** Main expansion function: handles quotes and variable expansion.
** Processes the input string character by character:
** - Single quotes ('): no expansion, remove quotes
** - Double quotes ("): expand variables, remove quotes
** - Dollar sign ($): expand variable
** - Other: copy literally
**
** Returns a newly allocated string with all expansions performed
** and quotes removed.
**
** @param s    Input string to expand
** @param mini Shell structure containing exit status and environment
** @return     Newly allocated expanded string, or NULL on malloc failure
*/
char	*expand_word(const char *s, t_shell *mini)
{
	t_buf_ctx	ctx;
	size_t		i;

	ctx.cap = ft_strlen(s) + 128;
	ctx.buf = malloc(ctx.cap);
	if (!ctx.buf)
		return (NULL);
	ctx.len = 0;
	i = 0;
	while (s[i])
	{
		if (!process_char(s, &i, &ctx, mini))
			return (free(ctx.buf), NULL);
	}
	ctx.buf[ctx.len] = '\0';
	return (ctx.buf);
}

/*
** Expands a single STRING token.
** Handles heredoc delimiters specially, otherwise performs full expansion.
**
** @param current Current token to expand
** @param prev    Previous token (to check for T_HEREDOC)
** @param mini    Shell structure
** @return        0 on success, 1 on error
*/
static int	expand_single_token(t_token *current, t_token *prev, t_shell *mini)
{
	char	*new;

	if (prev && prev->type == T_HEREDOC)
	{
		if (handle_heredoc(current) != 0)
			return (1);
	}
	else
	{
		new = expand_word(current->value, mini);
		if (!new)
			return (1);
		free(current->value);
		current->value = new;
	}
	return (0);
}

/*
** Expands all STRING tokens in the token list.
** Handles two cases:
** 1. Heredoc delimiters: strip quotes, set heredoc_no_expand flag
** 2. Normal words: full expansion (quotes + variables)
**
** The previous token is tracked to identify heredoc delimiters
** (tokens that follow a T_HEREDOC token).
**
** @param mini Shell structure containing token list
** @return     0 on success, 1 on error
*/
int	expand_main(t_shell *mini)
{
	t_token	*current;
	t_token	*prev;

	current = mini->tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == T_STRING)
		{
			if (expand_single_token(current, prev, mini) != 0)
				return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
