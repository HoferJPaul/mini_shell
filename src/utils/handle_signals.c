/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:08:33 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/05 13:59:10 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//ctrl-c, ctrl-d, ctrl-'\' handling functions here
//Handles SIGINT (Ctrl+C is printed and ignored)
void	sigint_handler(int signo)
{
	(void)signo;
	g_exit_status = (130);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//Sets up signal handlers: SIGINT → custom handler, SIGQUIT → ignored.
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
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
