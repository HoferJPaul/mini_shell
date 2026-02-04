/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:06:52 by phofer            #+#    #+#             */
/*   Updated: 2026/02/04 12:02:32 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

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

//splits args by spaces (" ")
static char	**split_args(char *line)
{
	return (ft_split(line, ' '));
}

//frees malloc'd jagged array
static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

//verifies str is not empty line (whitespaces)
static int	is_empy_line(const char *str)
{
	while (*str)
	{
		if (!ft_isspace)
			return (0);
		++str;
	}
	return (1);
}

//simple version of exec for testing
/*static void	exec_simple(char	**args, char **envp)
{
	pid_t	pid;
	int		status;

}


static void	process_line(char *line, char **envp)
{
}*/

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
			add_history(input);
	//	process_line(input, envp);
		free(input);
	}
	return (g_exit_status);
}
