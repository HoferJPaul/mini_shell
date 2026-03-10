/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:11:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:21:48 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

// Joins dir + "/" + cmd and returns full path if executable, else NULL.
static char	*try_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (access(full, X_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

// Searches PATH dirs for an executable matching cmd; returns path or NULL.
char	*find_command_path(t_shell *mini, char *cmd)
{
	char	**dirs;
	char	*full_path;
	int		i;

	if (!env_get(mini->env, "PATH"))
		return (NULL);
	dirs = ft_split(env_get(mini->env, "PATH"), ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = try_path(dirs[i], cmd);
		if (full_path)
		{
			free_array(dirs);
			return (full_path);
		}
		i++;
	}
	free_array(dirs);
	return (NULL);
}

// Handles execve failure: prints error, frees resources, exits 126 or 127.
static void	exec_error(char *cmd, char *path, char **envp)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (errno == ENOENT)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else
		perror(cmd);
	if (path != cmd)
		free(path);
	free_array(envp);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

/*
** Resolves and executes an external command via execve (call from child).
** Uses PATH lookup unless cmd contains '/'. Exits on any failure.
*/
void	exec_external(t_shell *mini, char **command)
{
	char	*path;
	char	**envp;

	if (ft_strchr(command[0], '/'))
		path = command[0];
	else
		path = find_command_path(mini, command[0]);
	if (!path)
	{
		ft_putstr_fd(command[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_array(mini->env);
	execve(path, command, envp);
	exec_error(command[0], path, envp);
}
