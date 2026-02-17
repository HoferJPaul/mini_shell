/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/17 17:03:07 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/16 00:00:00 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

/*
** Checks if token is a redirection operator
*/
static int	is_redirect(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND || type == T_HEREDOC);
}

/*
** Checks if token is an operator (pipe or redirect)
*/
static int	is_operator_token(t_token_type type)
{
	return (type == T_PIPE || is_redirect(type));
}

/*
** Validates that a redirect operator is followed by a filename
**
** @param token The redirect operator token
** @return      0 on success, 1 on syntax error
*/
static int	validate_redirect(t_token *token)
{
	if (!token->next)
	{
		ft_putstr_fd("syntax error: expected file after redirect\n", 2);
		return (1);
	}
	if (token->next->type != T_STRING)
	{
		ft_putstr_fd("syntax error: expected filename after redirect\n", 2);
		return (1);
	}
	return (0);
}

/*
** Validates token sequence for syntax errors
**
** Checks for:
** - Pipe at start or end
** - Double pipes
** - Redirect without filename
** - Operator after redirect
**
** @param tokens Token list to validate
** @return       0 if valid, 1 if syntax error
*/
int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	if (tokens->type == T_PIPE)
		return (ft_putstr_fd("syntax error: unexpected pipe\n", 2), 1);
	current = tokens;
	while (current)
	{
		if (is_redirect(current->type))
		{
			if (validate_redirect(current) != 0)
				return (1);
			current = current->next;
		}
		else if (current->type == T_PIPE)
		{
			if (!current->next)
				return (ft_putstr_fd("syntax error: unexpected pipe\n", 2), 1);
			if (current->next->type == T_PIPE)
				return (ft_putstr_fd("syntax error: unexpected pipe\n", 2), 1);
		}
		current = current->next;
	}
	return (0);
}

/*
** Main parser entry point
** Validates syntax and converts tokens into command structure
**
** @param mini Shell structure containing tokens
** @return     0 on success, 1 on error
*/
int	parse_main(t_shell *mini)
{
	if (!mini || !mini->tokens)
		return (0);
	if (validate_syntax(mini->tokens) != 0)
	{
		mini->g_exit_status = 2;
		return (1);
	}
	mini->commands = parse_pipeline(mini->tokens);
	if (!mini->commands)
	{
		mini->g_exit_status = 1;
		return (1);
	}
	return (0);
}
