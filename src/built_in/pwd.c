/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:39 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:00:49 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*@brief
// Replica of the pwd builtin: prints the current working directory
// stored in the shell context. If the directory is not available,
// prints an error message and returns.
*/
int	pwd(t_shell *mini)
{
	if (!mini->cwd)
	{
		perror("cwd NULL!");
		return (1);
	}
	printf("%s\n", mini->cwd);
	return (0);
}
