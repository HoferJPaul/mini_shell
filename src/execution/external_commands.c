/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:11:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 16:28:07 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

char	*find_command_path(t_shell *mini, char *cmd)
{
	char	*path;
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	path = env_get(mini->env, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(dirs); /* fix: was leaking dirs on success */
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}

/* just executing it as it is, doesn't check if there's a pipe,
   assumes already being called in the child process */
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
	perror("execve");
	if (path != command[0])
		free(path);
	free_array(envp);
	exit(126);
}
