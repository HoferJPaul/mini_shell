/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:08:33 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/09 18:37:01 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// ctrl-c, ctrl-d, ctrl-'\' handling functions here
// Handles SIGINT (Ctrl+C is printed and ignored)
void	sigint_prompt(int signo)
{
	(void)signo;
	g_sigint_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_exec(int signo)
{
	(void)signo;
	g_sigint_received = 1;
	write(1, "\n", 1);
}

void	setup_prompt_signals(void)
{
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_exec_signals(void)
{
	signal(SIGINT, sigint_exec);
	signal(SIGQUIT, SIG_IGN);
}

// technically this is not a signal we can catch, so we are catching the EOF
void	ctrl_d(char *line, t_shell *mini)
{
	int	status;

	if (!line)
	{
		printf("exit\n");
		status = mini->g_exit_status;
		free_dobby(mini);
		exit(status);
	}
}
