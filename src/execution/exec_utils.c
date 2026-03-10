/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 16:14:28 by phofer            #+#    #+#             */
/*   Updated: 2026/03/10 16:14:48 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

/*
** Counts the number of commands in the linked list.
** Used to determine how many PIDs to allocate for the pipeline.
**
** @param commands: Head of the command linked list
** @return: Number of commands in the list
*/
int	count_commands(t_command *commands)
{
	t_command	*curr;
	int			i;

	i = 0;
	curr = commands;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}
