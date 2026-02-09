/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:08:33 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/09 17:32:45 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//ctrl-c, ctrl-d, ctrl-'\' handling functions here
//Handles SIGINT (Ctrl+C is printed and ignored)
void sigint_handler(int signo)
{
	(void)signo;
	g_sigint_received = 1;

	//TODO apparently these functions are unsafe here
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//Sets up signal handlers: SIGINT → custom handler, SIGQUIT → ignored.
void	setup_signals(t_shell *mini)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (g_sigint_received)
	{
		g_sigint_received = 0;
		mini->g_exit_status = 130;
		write(1, "\n", 1);
	}
}
void    ctrl_d(char *line)//technically this is not a signal we can catch, so we are catching the EOF
{
    if(!line)
    {
        printf("exit\n");
        //cleanup stuff here
        exit(0);
    }
}
