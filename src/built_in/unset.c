/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:52 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/16 17:08:21 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
static int validate_var(char *var)
{
     int i;

    if (var[0] && (var[0] == '_' || ft_isalpha(var[0]) == 1))
    {
        i = 1;
        while (var[i])
        {
            if (var[i] != '_' && ft_isalnum(var[i]) != 1)
            {
                ft_putstr_fd("export: not a valid identifier\n", 2);
                return -1;
            }
            i++;
        }
        return 1;
    }
    ft_putstr_fd("export: not a valid identifier\n", 2);
    return -1;
}
//should return 0 on success and 1 on fail.
int unset(t_env *env, char **vars)
{
    
}