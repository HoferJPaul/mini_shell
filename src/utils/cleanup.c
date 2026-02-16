/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:09:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/10 14:52:10 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//all the frees etc here
//should free the add_history with clear_history here
void free_array(char **arr)
{
    int i = 0;

    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
