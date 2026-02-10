/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:07 by phofer            #+#    #+#             */
/*   Updated: 2026/02/09 17:30:35 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/parser.h"

static size_t	find_closing_quotes(const char *s, size_t i, char q)
{
	size_t	j;

	j = i + 1;
	while (s[j] && s[j] != q)
		j++;
	if (!s[j])
		return (SIZE_MAX);
	return (j);
}

static int	measure_word(const char *s, size_t i, t_word_range *range)
{
	size_t	start;
	size_t	j;

	range->len = 0;
	start = i;
	while (s[i])
	{
		if (ft_isspace(s[i]) || is_operator(s[i]))
			break ;
		if (is_quote(s[i]))
		{
			j = find_closing_quotes(s, i, s[i]);
			if (j == SIZE_MAX)
				return (1);
			i = j + 1;
		}
		else
			i++;
	}
	range->end = i;
	range->len = range->end - start;
	return (0);
}

int	tokenize_word(t_token **tokens, const char *str, size_t *i)
{
	t_token			*t;
	t_word_range	range;
	size_t			start;

	start = *i;
	if (measure_word(str, start, &range) != 0)
		return (1);
	t = new_token(T_STRING, str + start, range.len);
	if (!t)
		return (-1);
	add_token(tokens, t);
	*i = range.end;
	return (0);
}
