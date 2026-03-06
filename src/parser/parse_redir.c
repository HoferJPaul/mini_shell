/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/06 16:06:22 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

/*
** Counts how many STRING tokens before next pipe or end
**
** @param tokens Starting token
** @return       Count of argument tokens
*/
static int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_STRING)
			count++;
		else if (tokens->type == T_REDIR_IN || tokens->type == T_REDIR_OUT
			|| tokens->type == T_APPEND || tokens->type == T_HEREDOC)
		{
			tokens = tokens->next;
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

/*
** Builds a NULL-terminated array of arguments from tokens
** Skips redirect operators and their targets
**
** @param tokens Starting token
** @return       Allocated string array, or NULL on failure
*/
char	**build_args_array(t_token *tokens)
{
	char	**args;
	int		count;
	int		i;

	count = count_args(tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_STRING)
		{
			args[i] = ft_strdup(tokens->value);
			if (!args[i])
				return (NULL);
			i++;
		}
		else if (tokens->type == T_REDIR_IN || tokens->type == T_REDIR_OUT
			|| tokens->type == T_APPEND || tokens->type == T_HEREDOC)
			tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
	}
	args[i] = NULL;
	return (args);
}

/*
** Checks if token is a redirection operator
*/
static int	is_redirect_type(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND || type == T_HEREDOC);
}

/*
** Adds a redirect to the end of the redirect list
**
** @param head Pointer to head of redirect list
** @param new  New redirect to add
*/
static void	add_redirect(t_redirect **head, t_redirect *new)
{
	t_redirect	*current;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

/*
** Parses redirections from token list for a single command
** Collects all redirects before next pipe or end
** Just stores pointers to tokens - no data duplication
**
** @param tokens Starting token
** @return       Head of redirect list, or NULL if none
*/
t_redirect	*parse_redirects(t_token *tokens)
{
	t_redirect	*redirects;
	t_redirect	*new_redir;

	redirects = NULL;
	while (tokens && tokens->type != T_PIPE)
	{
		if (is_redirect_type(tokens->type))
		{
			if (!tokens->next)
				return (free_redirects(redirects), NULL);
			new_redir = init_redirect(tokens, tokens->next);
			if (!new_redir)
				return (free_redirects(redirects), NULL);
			add_redirect(&redirects, new_redir);
			tokens = tokens->next;
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (redirects);
}
