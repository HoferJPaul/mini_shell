/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:08:33 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 16:53:39 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <termios.h>

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

// Suppress or restore terminal echoing of control characters (e.g. ^C).
void	termios_change(int echo_ctl_chr)
{
	struct termios	t;

	if (tcgetattr(STDOUT_FILENO, &t) == -1)
		return ;
	if (echo_ctl_chr)
		t.c_lflag |= ECHOCTL;
	else
		t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &t);
}

// Sets up signal handlers: SIGINT → custom handler, SIGQUIT → ignored.
void	setup_signals(t_shell *mini)
{
	(void) mini;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ctrl_d(char *line)
{
	if (!line)
	{
		printf("exit\n");
		exit(0);
	}
}
