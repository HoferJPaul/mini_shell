/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/23 17:22:38 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/command.h"
#include "../include/debug.h"
#include "../include/parser.h"
#include <stdio.h>

/*
** Converts token type enum to readable string
**
** @param type Token type enum
** @return     String representation of type
*/
static const char	*token_type_to_string(t_token_type type)
{
	if (type == T_STRING)
		return ("T_STRING");
	if (type == T_PIPE)
		return ("T_PIPE");
	if (type == T_REDIR_IN)
		return ("T_REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("T_REDIR_OUT");
	if (type == T_APPEND)
		return ("T_APPEND");
	if (type == T_HEREDOC)
		return ("T_HEREDOC");
	return ("T_NONE");
}

/*
** Prints a single token's information
**
** @param token     Token to print
** @param index     Token index in list
** @param show_addr Whether to show memory address
*/
void	print_single_token(t_token *token, int index, int show_addr)
{
	printf("  Token[%d]:\n", index);
	printf("    type:              %s\n", token_type_to_string(token->type));
	printf("    value:             \"%s\"\n", token->value);
	printf("    heredoc_no_expand: %d\n", token->heredoc_no_expand);
	if (show_addr)
	{
		printf("    address:           %p\n", (void *)token);
		printf("    next:              %p\n", (void *)token->next);
	}
}

/*
** Prints all tokens in a linked list
** Shows each token's type, value, heredoc flag, and position
**
** @param tokens Head of token list
*/
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		count;
	int		i;

	if (!tokens)
	{
		printf("Token list: (empty)\n");
		return ;
	}
	current = tokens;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   TOKEN LIST (%d tokens)               \n", count);
	printf("╚════════════════════════════════════════╝\n");
	current = tokens;
	i = 0;
	while (current)
	{
		print_single_token(current, i, 0);
		printf("\n");
		current = current->next;
		i++;
	}
}

/*
** Prints tokens in a compact single-line format
** Useful for quick debugging
**
** Example: [echo] ["hello world"] [|] [grep] [test]
**
** @param tokens Head of token list
*/
void	print_tokens_compact(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
	{
		printf("Tokens: (empty)\n");
		return ;
	}
	printf("Tokens: ");
	current = tokens;
	while (current)
	{
		printf("[%s", token_type_to_string(current->type));
		if (current->value && current->value[0])
			printf(": \"%s\"", current->value);
		printf("]");
		if (current->next)
			printf(" ");
		current = current->next;
	}
	printf("\n");
}

/*
** Prints tokens with visual separators for pipelines
** Shows pipeline structure clearly
**
** Example:
**   Pipeline segment 1:
**     [T_STRING: "echo"] [T_STRING: "hello"]
**   |
**   Pipeline segment 2:
**     [T_STRING: "grep"] [T_STRING: "test"]
**
** @param tokens Head of token list
*/
void	print_tokens_pipeline(t_token *tokens)
{
	t_token	*current;
	int		segment;

	if (!tokens)
	{
		printf("Tokens: (empty)\n");
		return ;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   TOKEN PIPELINE VIEW                  ║\n");
	printf("╚════════════════════════════════════════╝\n");
	current = tokens;
	segment = 1;
	printf("\nPipeline segment %d:\n  ", segment);
	while (current)
	{
		if (current->type == T_PIPE)
		{
			printf("\n  " YELLOW "│" RESET "\n");
			segment++;
			printf("Pipeline segment %d:\n  ", segment);
		}
		else
		{
			printf("[%s", token_type_to_string(current->type));
			if (current->value && current->value[0])
				printf(": \"%s\"", current->value);
			printf("] ");
		}
		current = current->next;
	}
	printf("\n");
}

/*
** Prints detailed token information including memory addresses
** Useful for debugging memory issues
**
** @param tokens Head of token list
*/
void	print_tokens_debug(t_token *tokens)
{
	t_token	*current;
	int		i;

	if (!tokens)
	{
		printf("Token list: (empty)\n");
		return ;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   TOKEN LIST (DEBUG MODE)              ║\n");
	printf("╚════════════════════════════════════════╝\n");
	current = tokens;
	i = 0;
	while (current)
	{
		printf("\n");
		print_single_token(current, i, 1);
		current = current->next;
		i++;
	}
	printf("\nTotal tokens: %d\n", i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_commands.c                                   :+:      :+:    :+:   */
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
#include "../include/debug.h"
#include <stdio.h>

/*
** Converts token type to operator symbol
*/
static const char	*get_redirect_symbol(t_token_type type)
{
	if (type == T_REDIR_IN)
		return ("<");
	if (type == T_REDIR_OUT)
		return (">");
	if (type == T_APPEND)
		return (">>");
	if (type == T_HEREDOC)
		return ("<<");
	return ("?");
}

/*
** Prints a single command with all details (concise format)
*/
static void	print_single_command(t_command *cmd, int cmd_num)
{
	t_redirect	*redir;
	int			i;

	printf("  Command[%d]:\n", cmd_num);
	printf("    args:      ");
	if (!cmd->args || !cmd->args[0])
		printf("(none)");
	else
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("\"%s\"", cmd->args[i]);
			if (cmd->args[i + 1])
				printf(", ");
			i++;
		}
	}
	printf("\n");
	printf("    redirects: ");
	if (!cmd->redirects)
		printf("(none)");
	else
	{
		redir = cmd->redirects;
		while (redir)
		{
			printf("%s %s", get_redirect_symbol(redir->redir_token->type),
				redir->file_token->value);
			if (redir->redir_token->type == T_HEREDOC)
			{
				if (redir->file_token->heredoc_no_expand)
					printf(" [no_expand]");
				else
					printf(" [expand]");
			}
			if (redir->next)
				printf(", ");
			redir = redir->next;
		}
	}
	printf("\n");
	printf("    next:      %p\n", (void *)cmd->next);
}

/*
** Prints all commands in a command list (standard view)
**
** @param commands Head of command list
*/
void	print_commands(t_command *commands)
{
	t_command	*current;
	int			i;

	if (!commands)
	{
		printf("Command list: (empty)\n");
		return ;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   COMMAND LIST (DEBUG MODE)            ║\n");
	printf("╚════════════════════════════════════════╝\n");
	current = commands;
	i = 0;
	while (current)
	{
		print_single_command(current, i);
		if (current->next)
			printf("\n");
		current = current->next;
		i++;
	}
}

/*
** Prints commands in compact one-line format
** Example: [echo hello] | [grep test] > out.txt
*/
void	print_commands_compact(t_command *commands)
{
	t_command	*current;
	t_redirect	*redir;
	int			i;

	if (!commands)
	{
		printf("Commands: (empty)\n");
		return ;
	}
	printf("Commands: ");
	current = commands;
	while (current)
	{
		printf("[");
		if (current->args && current->args[0])
		{
			i = 0;
			while (current->args[i])
			{
				printf("%s", current->args[i]);
				if (current->args[i + 1])
					printf(" ");
				i++;
			}
		}
		printf("]");
		redir = current->redirects;
		while (redir)
		{
			printf(" %s %s", get_redirect_symbol(redir->redir_token->type),
				redir->file_token->value);
			redir = redir->next;
		}
		if (current->next)
			printf(" " YELLOW "|" RESET " ");
		current = current->next;
	}
	printf("\n");
}

/*
** Prints commands with pipeline visualization
*/
void	print_commands_pipeline(t_command *commands)
{
	t_command	*current;
	t_redirect	*redir;
	int			cmd_num;
	int			i;

	if (!commands)
	{
		printf("Commands: (empty)\n");
		return ;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   COMMAND PIPELINE                     ║\n");
	printf("╚════════════════════════════════════════╝\n\n");
	current = commands;
	cmd_num = 1;
	while (current)
	{
		printf(BOLD "Command %d:" RESET " ", cmd_num);
		if (current->args && current->args[0])
		{
			i = 0;
			while (current->args[i])
			{
				printf("%s", current->args[i]);
				if (current->args[i + 1])
					printf(" ");
				i++;
			}
		}
		printf("\n");
		if (current->redirects)
		{
			printf("  Redirects: ");
			redir = current->redirects;
			while (redir)
			{
				printf("%s %s", get_redirect_symbol(redir->redir_token->type),
					redir->file_token->value);
				if (redir->next)
					printf(", ");
				redir = redir->next;
			}
			printf("\n");
		}
		if (current->next)
			printf("  " YELLOW "    ↓ pipe\n" RESET);
		current = current->next;
		cmd_num++;
	}
	printf("\n");
}

/*
** Prints commands with memory addresses (debug mode)
*/
void	print_commands_debug(t_command *commands)
{
	t_command	*current;
	t_redirect	*redir;
	int			i;
	int			j;

	if (!commands)
	{
		printf("Commands: (empty)\n");
		return ;
	}
	printf("╔════════════════════════════════════════╗\n");
	printf("║   COMMAND LIST (DEBUG MODE)            ║\n");
	printf("╚════════════════════════════════════════╝\n");
	current = commands;
	i = 1;
	while (current)
	{
		printf("\n" BOLD "Command %d:" RESET "\n", i);
		printf("  address:   %p\n", (void *)current);
		printf("  next:      %p\n", (void *)current->next);
		printf("  args:      %p\n", (void *)current->args);
		if (current->args)
		{
			j = 0;
			while (current->args[j])
			{
				printf("    [%d] %p: \"%s\"\n", j,
					(void *)current->args[j], current->args[j]);
				j++;
			}
		}
		printf("  redirects: %p\n", (void *)current->redirects);
		if (current->redirects)
		{
			redir = current->redirects;
			j = 0;
			while (redir)
			{
				printf("    [%d] %p:\n", j, (void *)redir);
				printf("      redir_token: %p (type: %s)\n",
					(void *)redir->redir_token,
					get_redirect_symbol(redir->redir_token->type));
				printf("      file_token:  %p (value: \"%s\")\n",
					(void *)redir->file_token,
					redir->file_token->value);
				printf("      heredoc_content: %p\n",
					(void *)redir->heredoc_content);
				printf("      next:        %p\n", (void *)redir->next);
				redir = redir->next;
				j++;
			}
		}
		current = current->next;
		i++;
	}
	printf("\nTotal commands: %d\n", i - 1);
}

/*
** Prints command in shell-like format (reconstructed input)
*/
void	print_commands_shell_format(t_command *commands)
{
	t_command	*current;
	t_redirect	*redir;
	int			i;

	if (!commands)
	{
		printf("(empty)\n");
		return ;
	}
	current = commands;
	while (current)
	{
		if (current->args && current->args[0])
		{
			i = 0;
			while (current->args[i])
			{
				printf("%s", current->args[i]);
				if (current->args[i + 1])
					printf(" ");
				i++;
			}
		}
		redir = current->redirects;
		while (redir)
		{
			printf(" %s %s", get_redirect_symbol(redir->redir_token->type),
				redir->file_token->value);
			redir = redir->next;
		}
		if (current->next)
			printf(" | ");
		current = current->next;
	}
	printf("\n");
}
void	process_line_debug(t_shell *mini, char *input)
{
	int	err;

	err = tokenize(mini, input);
	//TESTING=====================
	printf("\nTESTING: TOKENIZATION\n");
	print_tokens_debug(mini->tokens);
	//TESTING=====================
	if (err)
		return ((void)(mini->g_exit_status = err));
	err = expand(mini);
	//TESTING=====================
	printf("\nTESTING: EXPANSION\n");
	print_tokens_debug(mini->tokens);
	//TESTING=====================
	if (err)
		return ((void)(mini->g_exit_status = err));
	err = parse(mini);
	//TESTING=====================
	printf("\nTESTING: COMMAND STRUCT\n");
	print_commands_debug(mini->commands);
	//TESTING=====================
	if (err)
		return ((void)(mini->g_exit_status = err));
	// err = execute(mini);
	// if (err)
		// mini->g_exit_status = 1;
}
