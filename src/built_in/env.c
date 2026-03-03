/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:55:19 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/03 15:02:44 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*@brief
// Replica of the env builtin: prints all environment variables
// from the linked list. Displays each entry as "key=value" if a
// value exists, otherwise only the key. Iterates through all nodes.
//already tested, should work without an issue.
*/
int	env(t_env *env)
{
	t_env *cp;
    
    cp = env;
	while (cp)
	{
		if (cp->value)
			printf("%s=%s\n", cp->key, cp->value);
		cp = cp->next;
	}
	return (0);
}