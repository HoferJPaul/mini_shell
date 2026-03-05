/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:08:33 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 16:59:43 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// ctrl-c, ctrl-d, ctrl-'\' handling functions here
// Handles SIGINT (Ctrl+C is printed and ignored)
void	sigint_handler(int signo)
{
	(void)signo;
	g_sigint_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
}

/*
** SIGINT handler used only during heredoc readline loop.
** Sets the global flag and makes readline return NULL on next call
** by closing stdin — clean, no forbidden functions needed.
*/
void	heredoc_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	// close(STDIN_FILENO);
}

// Sets up signal handlers: SIGINT → custom handler, SIGQUIT → ignored.
void	setup_signals(t_shell *mini)
{
	(void) mini;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
void	ctrl_d(char *line, t_shell *mini) // technically this is not a signal we can catch, so we are catching the EOF
{
	if (!line)
	{
		printf("exit\n");
		free_dobby(mini);
		exit(0);
	}
}
