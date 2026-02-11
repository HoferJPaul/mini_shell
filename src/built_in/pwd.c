/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:52:39 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/11 13:39:05 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*@brief
// Replica of the pwd builtin: prints the current working directory
// stored in the shell context. If the directory is not available,
// prints an error message and returns.
*/
void pwd(t_shell *mini)
{
    if(!mini->cwd)
    {
        perror("cwd NULL!");
        return;
    }
    printf("%s\n", mini->cwd);
}
