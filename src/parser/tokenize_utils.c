/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:32:40 by phofer            #+#    #+#             */
/*   Updated: 2026/02/09 17:29:40 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/tokens.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_whitespaces(const char *str, size_t *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
}
