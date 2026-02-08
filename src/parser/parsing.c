/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:32:40 by phofer            #+#    #+#             */
/*   Updated: 2026/02/08 13:36:59 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"

t_token_type	get_operator_type(const char *s)
{
	if (s[0] == '<' && s[1] == '<')
		return (T_HEREDOC);
	if (s[0] == '>' && s[1] == '>')
		return (T_APPEND);
	if (s[0] == '<')
		return (T_REDIR_IN);
	if (s[0] == '>')
		return (T_REDIR_OUT);
	return (T_PIPE);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_whitespaces(const char *str, size_t *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
}

int	lex_line(t_shell *mini, const char *input)
{
	size_t	i;

	if (!input)
		return (-1);

	mini->tokens = NULL;
	i = 0;
	while (input[i] && input[i] != '\n')
	{
		skip_whitespaces(input, &i);
		if (!input[i] && input[i] != '\n')
			break ;
		if (is_operator(input[i]))
			tokenize_operator(mini->tokens, input, &i); //todo
		else if (!tokenize_word(mini->tokens, input, &i))
		{
			//print error;
			return (0);
		}
	}
	return (1);
}
