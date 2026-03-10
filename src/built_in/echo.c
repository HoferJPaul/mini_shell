/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 14:57:48 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

// Prints args separated by spaces, no newline.
static void	echo_print_args(char **command, int i)
{
	while (command[i])
	{
		ft_putstr_fd(command[i], STDOUT_FILENO);
		if (command[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

/*
** echo builtin. Prints command args to stdout separated by spaces.
** Supports -n flag (suppress trailing newline). No arg prints newline.
*/
int	echo(char **command)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	if (command[i] && ft_strcmp(command[i], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	if (!command[1])
	{
		if (flag == 0)
			ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	echo_print_args(command, i);
	if (flag == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
