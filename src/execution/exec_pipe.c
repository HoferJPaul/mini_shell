/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:12:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/03 14:33:41 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int	count_commands(t_command *commands)
{
	t_command	*curr;
	int			i;

	i = 0;
	curr = commands;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

int	extract_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	exec_pipe_child(t_shell *mini, t_command *curr, int prev_fd,
		int fd[2])
{
	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (curr->next)
		dup2(fd[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (curr->next)
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (check_builtin(curr->args[0]) == 0)
		exit(exec_builtin(mini, curr->args));
	exec_external(mini, curr->args);
	perror("execve");
	exit(127);
}

static int	execute_pipe_cmds(t_shell *mini, pid_t *pid, pid_t *last_pid)
{
	t_command	*curr;
	int			prev_fd;
	int			fd[2];
	int			i;

	curr = mini->commands;
	prev_fd = -1;
	i = 0;
	while (curr)
	{
		if (curr->next && pipe(fd) == -1)
			return (perror("pipe"), -1);
		pid[i] = fork();
		if (pid[i] == -1)
			return (perror("fork"), -1);
		if (pid[i] == 0)
			exec_pipe_child(mini, curr, prev_fd, fd);
		if (prev_fd != -1)
			close(prev_fd);
		if (curr->next)
			close(fd[1]), prev_fd = fd[0];
		curr = curr->next;
		i++;
	}
	*last_pid = pid[i - 1];
	return (i);
}

int	execution_pipe(t_shell *mini)
{
	pid_t	*pid;
	pid_t	last_pid;
	int		status;
	int		count;
	int		i;

	pid = malloc(count_commands(mini->commands) * sizeof(pid_t));
	if (!pid)
		return (1);
	count = execute_pipe_cmds(mini, pid, &last_pid);
	if (count == -1)
		return (free(pid), 1);
	i = 0;
	while (i < count)
	{
		waitpid(pid[i], &status, 0);
		if (pid[i] == last_pid)
			mini->g_exit_status = extract_status(status);
		i++;
	}
	free(pid);
	return (0);
}
