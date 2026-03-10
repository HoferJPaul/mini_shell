/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:47:24 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:39:06 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// to initialize the path at first
void	init_paths(t_env *env, t_shell *mini)
{
	char	*path_value;

	path_value = env_get(env, "PATH");
	if (path_value)
		mini->paths = ft_split(path_value, ':');
	else
		mini->paths = NULL;
}

// only called in unset and export
void	refresh_paths(t_shell *mini)
{
	char	*path_value;

	if (mini->paths)
		free_array(mini->paths);
	path_value = env_get(mini->env, "PATH");
	if (path_value)
		mini->paths = ft_split(path_value, ':');
	else
		mini->paths = NULL;
}

char	*path_append(char *old, char *add)
{
	char	*tmp;
	char	*res;

	if (!old)
		return (ft_strdup(add));
	tmp = ft_strjoin(old, ":");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, add);
	free(tmp);
	return (res);
}

// value of new_path should be recieved in right format
void	add_new_paths(t_shell *mini, char *new_path)
{
	char	*old;
	char	*new_value;

	old = env_get(mini->env, "PATH");
	new_value = path_append(old, new_path);
	if (!new_value)
		return ;
	env_set(&mini->env, "PATH", new_value, 0);
	free(new_value);
	refresh_paths(mini);
}
