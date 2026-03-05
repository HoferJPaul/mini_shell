/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:12:51 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 16:51:05 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*env_get(t_env *env, char *key) // we'll use this instead of getenv
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	env_set(t_env **env, char *key, char *value, int exported_flag)
// set/update the env
{
	t_env *tmp = *env;

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
	// not found → add new
	t_env *new = create_env(key, value, exported_flag);
	new->next = *env;
	*env = new;
}

int	env_size(t_env *env) // pretty straightforward name xd
{
	int i = 0;
	while (env && ++i)
		env = env->next;
	return (i);
}

char	**env_to_array(t_env *env) // because execve needs char **
{
	int n;
	char **arr;
	int i;
	char *tmp;
    int val_len;
    
    n = env_size(env);
	arr = malloc(sizeof(char *) * (n + 1));
    if(!arr)
        return NULL;
    i = 0;
	while (env)
	{
        if(env->value == NULL)
            val_len = 0;
        else
            val_len = ft_strlen(env->value);
		tmp = malloc(ft_strlen(env->key) + val_len + 2);
        if(!tmp)
        {
            arr[i] = NULL;
            free_array(arr);
            return NULL;
        }
        if(env->value == NULL)
            env->value = "";
		sprintf(tmp, "%s=%s", env->key, env->value);
		arr[i++] = tmp;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
