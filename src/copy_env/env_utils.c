/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:12:51 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:12:09 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// we'll use this instead of getenv
char	*env_get(t_env *env, char *key)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

// set/update the env
void	env_set(t_env **env, char *key, char *value, int exported_flag)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			tmp->exported_flag = exported_flag;
			return ;
		}
		tmp = tmp->next;
	}
	new = create_env(key, value, exported_flag);
	new->next = *env;
	*env = new;
}

// pretty straightforward name xd
int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env && ++i)
		env = env->next;
	return (i);
}

// Builds a "key=value" heap string for one env node.
static char	*env_node_to_str(t_env *env)
{
	char	*tmp;
	int		val_len;

	if (env->value == NULL)
		val_len = 0;
	else
		val_len = ft_strlen(env->value);
	tmp = malloc(ft_strlen(env->key) + val_len + 2);
	if (!tmp)
		return (NULL);
	if (env->value == NULL)
		env->value = "";
	sprintf(tmp, "%s=%s", env->key, env->value);
	return (tmp);
}

// Converts the env linked list
//		to a NULL-terminated array of "key=value" strings.
char	**env_to_array(t_env *env)
{
	char	**arr;
	int		i;
	int		n;

	n = env_size(env);
	arr = malloc(sizeof(char *) * (n + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = env_node_to_str(env);
		if (!arr[i])
		{
			free_array(arr);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
