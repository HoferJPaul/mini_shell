/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:06:52 by phofer            #+#    #+#             */
/*   Updated: 2026/02/04 17:52:32 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/parser.h"

int			g_exit_status = 0;

//Handles SIGINT (Ctrl+C is printed and ignored)
static void	sigint_handler(int signo)
{
	(void)signo;
	g_exit_status = (130);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//Sets up signal handlers: SIGINT → custom handler, SIGQUIT → ignored.
static void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

//initiates parsing/tokenizing functions
// located in ../src/parser
static void	process_line(char *input)
{
	if (!lex_line(input))
		return ;
	// expand_tokens();
	// parse_tokens();
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			//process_line(input);
		}
		free(input);
	}
	return (g_exit_status);
}
