/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 15:36:02 by phofer            #+#    #+#             */
/*   Updated: 2026/03/10 16:32:55 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"

/*
** Adds a command to the end of the command list
**
** @param head Pointer to head of command list
** @param new  New command to add
*/
static void	add_command(t_command **head, t_command *new)
{
	t_command	*current;

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
** Parses a single command from tokens
** Collects arguments and redirections until pipe or end
**
** @param tokens Pointer to current token (will be updated)
** @return       Newly allocated command, or NULL on failure
*/
t_command	*parse_single_command(t_token **tokens)
{
	t_command	*cmd;
	t_token		*start;

	if (!tokens || !*tokens)
		return (NULL);
	start = *tokens;
	cmd = init_command();
	if (!cmd)
		return (NULL);
	cmd->args = build_args_array(start);
	if (!cmd->args)
		return (free_commands(cmd), NULL);
	cmd->redirects = parse_redirects(start);
	while (*tokens && (*tokens)->type != T_PIPE)
		*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == T_PIPE)
		*tokens = (*tokens)->next;
	return (cmd);
}

/*
** Parses entire pipeline from token list
** Creates linked list of commands separated by pipes
**
** @param tokens Token list
** @return       Head of command list, or NULL on failure
*/
t_command	*parse_pipeline(t_token *tokens)
{
	t_command	*commands;
	t_command	*cmd;

	commands = NULL;
	while (tokens)
	{
		cmd = parse_single_command(&tokens);
		if (!cmd)
			return (free_commands(commands), NULL);
		add_command(&commands, cmd);
	}
	return (commands);
}
