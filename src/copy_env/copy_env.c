/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:43:23 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:07:30 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// create the linked list node to put the key<->value pairs inside.
t_env	*create_env(char *key, char *value, int exported)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!value)
		node->value = NULL;
	else
		node->value = ft_strdup(value);
	node->exported_flag = exported;
	node->next = NULL;
	return (node);
}

// Duplicates at most n bytes of s into a new null-terminated string.
static char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

// parsing the key<->value pairs and putting them in the copy env node
t_env	*env_from_string(char *str)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;

	eq = ft_strchr(str, '=');
	if (!eq)
		return (create_env(str, NULL, 1));
	key = ft_strndup(str, eq - str);
	value = ft_strdup(eq + 1);
	node = create_env(key, value, 1);
	free(key);
	free(value);
	return (node);
}

// copying the envp inside our structure
t_env	*env_copy(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		node = env_from_string(envp[i]);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}
