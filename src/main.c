/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:06:52 by phofer            #+#    #+#             */
/*   Updated: 2026/02/05 13:58:13 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/parser.h"

//initiates parsing/tokenizing functions
// located in ../src/parser
// static void	process_line(char *input)
// {
// 	if (!lex_line(input))
// 		return ;
// 	// expand_tokens();
// 	// parse_tokens();
// }
int			g_exit_status = 0;

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
		ctrl_d(input);
		if (*input)
		{
			add_history(input);
			//process_line(input);
		}
		free(input);
	}
	return (g_exit_status);
}
