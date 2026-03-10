/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:37:48 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:12:21 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*temp;

	count = 0;
	temp = env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

static t_env	**build_env_array(t_env *env, int count)
{
	t_env	**arr;
	t_env	*temp;
	int		i;

	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return (NULL);
	temp = env;
	i = 0;
	while (temp)
	{
		arr[i] = temp;
		temp = temp->next;
		i++;
	}
	return (arr);
}

static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_env(t_env *env)
{
	t_env	**arr;
	int		count;
	int		i;

	if (!env)
		return ;
	count = count_env_nodes(env);
	arr = build_env_array(env, count);
	if (!arr)
		return ;
	sort_env_array(arr, count);
	i = 0;
	while (i < count)
	{
		if (arr[i]->exported_flag == 1)
		{
			if (arr[i]->value)
				printf("%s=%s\n", arr[i]->key, arr[i]->value);
			else
				printf("%s\n", arr[i]->key);
		}
		i++;
	}
	free(arr);
}
