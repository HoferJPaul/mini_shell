/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:06:52 by phofer            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/03/05 17:00:14 by zgahrama         ###   ########.fr       */
=======
/*   Updated: 2026/03/05 18:29:56 by phofer           ###   ########.fr       */
>>>>>>> debug_heredoc
/*                                                                            */
/* ************************************************************************** */

#include "../include/debug.h"
#include "../include/minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

static void	shell_loop(t_shell *mini)
{
	char	*input;

	while (mini->running)
	{
		input = readline("minishell> ");
		ctrl_d(input, mini);
		if (g_sigint_received != 0)
		{
			g_sigint_received = 0;
			mini->g_exit_status = 130;
		}
		if (*input)
		{
			add_history(input);
			process_line(mini, input);
			// process_line_debug(mini, input);
			if (g_sigint_received)
    		{
    		    g_sigint_received = 0;
    		    mini->g_exit_status = 130;
    		}
    		else
    		    execution(mini);
		}
		printf("TESTING: g_exit_status = %d\n", mini->g_exit_status);
		// for testing only
		free(input);
	}
}
int	main(int argc, char **argv, char **envp)
{
	t_shell	mini;
	t_env	*env;

	memset(&mini, 0, sizeof(t_shell));
	env = env_copy(envp);
	if (argc != 1)
	{
		ft_putstr_fd("Error: args forbidden", 2);
		return (1);
	}
	(void)argv;
	if (!setup_struct(&mini, &env))
		return (1);
	setup_signals(&mini);
	shell_loop(&mini);
	free_dobby(&mini);
	return (mini.g_exit_status);
}
