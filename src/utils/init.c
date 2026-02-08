/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:19:14 by phofer            #+#    #+#             */
/*   Updated: 2026/02/08 12:59:31 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include "../include/parser.h"

//initiates parsing/tokenizing functions - located in ../src/parser
void	process_line(t_shell *mini, char *input)
{
	if (!lex_line(&mini, input))
		return ;
	// expand_tokens();
	// parse_tokens();
}

//initialize all struct variables
int setup_struct(t_shell *mini, char **envp)
{
	mini->flag = 0;
	mini->g_exit_status = 1;
	mini->running = 1;
	mini->tokens = NULL;
	(void)envp;
	//TODO - env(envp)
	return (1);
}
