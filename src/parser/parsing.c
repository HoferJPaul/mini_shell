/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:32:40 by phofer            #+#    #+#             */
/*   Updated: 2026/02/06 17:43:09 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	skip_whitespaces(const char *str, size_t *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
}

int	lex_line(const char *input)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		skip_whitespaces(input, &i);
		if (!input[i])
			break ;
	}
	return (1);
}
