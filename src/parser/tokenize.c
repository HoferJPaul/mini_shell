/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:32:40 by phofer            #+#    #+#             */
/*   Updated: 2026/02/16 16:51:18 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/parser.h"

//determines operator type and len
static t_token_type	get_operator_info(const char *s, size_t *len)
{
	if (!s || !len)
		return (T_NONE);
	if (s[0] == '<' && s[1] == '<')
		return (*len = 2, T_HEREDOC);
	if (s[0] == '>' && s[1] == '>')
		return (*len = 2, T_APPEND);
	if (s[0] == '|')
		return (*len = 1, T_PIPE);
	if (s[0] == '<')
		return (*len = 1, T_REDIR_IN);
	if (s[0] == '>')
		return (*len = 1, T_REDIR_OUT);
	return (*len = 0, T_NONE);
}

//allocates a new token
t_token	*new_token(t_token_type type, const char *str, size_t len)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = malloc(sizeof(*token));
	if (!token)
		return (NULL);
	token->value = ft_substr(str, 0, len);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

//Adds token to linked list
void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

//handles operator
static	int	tokenize_operator(t_token **tokens, const char *str, size_t *i)
{
	t_token_type	type;
	size_t			len;

	if (!str || !i)
		return (-1);
	type = get_operator_info(&str[*i], &len);
	if (type == T_NONE)
		return (-1);
	add_token(tokens, new_token(type, &str[*i], len));
	*i += len;
	return (0);
}

int	tokenize_main(t_shell *mini, const char *input)
{
	size_t	i;

	if (!input)
		return (-1);
	mini->tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_whitespaces(input, &i);
		if (!input[i])
			break ;
		if (is_operator(input[i]))
		{
			if (tokenize_operator(&mini->tokens, input, &i) != 0)
				return (-1);
		}
		else if (tokenize_word(&mini->tokens, input, &i) != 0)
		{
			ft_putstr_fd("error: tokenization invalid\n", 2);
			return (1);
		}
	}
	return (0);
}
