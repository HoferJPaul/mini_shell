/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:06:52 by phofer            #+#    #+#             */
/*   Updated: 2026/02/08 15:07:43 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t g_sigint_received = 0;

static void shell_loop(t_shell *mini)
{
	char	*input;

	while (mini->running)
	{
		input = readline("minishell> ");
		ctrl_d(input);
		if (*input)
		{
			add_history(input);
			process_line_test(mini, input);
		}
		free(input);
	}
}
int	main(int argc, char **argv, char **envp)
{
	t_shell mini;
	memset(&mini, 0, sizeof(t_shell));

	if (argc != 1)
	{
		ft_putstr_fd("Error: args forbidden", 2);
		return (1);
	}
	(void)argv;
	if (!setup_struct(&mini, envp))
		return (1);
	setup_signals(&mini);
	shell_loop(&mini);

	//todo cleanup();
	return (mini.g_exit_status);
}
