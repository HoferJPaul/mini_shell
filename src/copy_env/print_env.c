/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:37:48 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/13 16:59:37 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;
	int		temp_flag;

	temp_key = a->key;
	temp_value = a->value;
	temp_flag = a->exported_flag;
	a->key = b->key;
	a->value = b->value;
	a->exported_flag = b->exported_flag;
	b->key = temp_key;
	b->value = temp_value;
	b->exported_flag = temp_flag;
}

t_env	*sort_env(t_env *env)
{
	t_env	*current;
	t_env	*index;

	if (!env)
		return (NULL);
	current = env;
	while (current)
	{
		index = current->next;
		while (index)
		{
			if (ft_strcmp(current->key, index->key) > 0)
				swap_env_nodes(current, index);
			index = index->next;
		}
		current = current->next;
	}
	return (env);
}
void print_env(t_env *env)
{
    t_env *temp;

    temp = env;
    while(temp)
    {
        if (temp->value)
            printf("%s=%s\n", temp->key, temp->value);
        else
            printf("%s\n", temp->key);
        temp = temp->next;
    }
}