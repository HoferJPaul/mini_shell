/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:12:09 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/05 13:24:08 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

/*
** Counts the number of commands in the linked list.
** Used to determine how many PIDs to allocate for the pipeline.
** 
** @param commands: Head of the command linked list
** @return: Number of commands in the list
*/
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

/*
** Extracts the exit status from waitpid() result.
** If process exited normally, returns exit code (0-255).
** If terminated by signal, returns 128 + signal number.
** This follows bash convention for signal exit codes.
**
** @param status: Status value returned by waitpid()
** @return: Normalized exit status for shell
*/
int	extract_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/*
** Executes a single command in a child process within a pipeline.
** Sets up pipe connections (stdin from prev_fd, stdout to fd[1]).
** Applies file redirections which can override pipe connections.
** Executes builtin or external command, exits on completion or error.
**
** @param mini: Shell state (environment, variables, etc)
** @param curr: Current command to execute
** @param prev_fd: Read end of previous pipe (-1 if first command)
** @param fd: Write end of current pipe (unused if last command)
** @return: Never returns (child process exits)
*/
static void	exec_pipe_child(t_shell *mini, t_command *curr, int prev_fd,
		int fd[2])
{
	/* wire up pipe fds BEFORE applying redirections so that explicit
	   redirections (< or >) can override the pipe ends if needed */
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
	/* apply file redirections – they override the pipe ends when present */
	if (apply_redirections(curr) == -1)
		exit(1);
	if (check_builtin(curr->args[0]) == 0)
		exit(exec_builtin(mini, curr->args));
	exec_external(mini, curr->args);
	/* exec_external never returns on success; reaching here means failure */
	exit(127);
}

/*
** Creates pipes and forks child processes for each command in pipeline.
** Each child gets connected via pipes, with stdin from previous command
** and stdout to next command. Parent closes unused pipe ends.
**
** @param mini: Shell state
** @param pid: Array to store child PIDs
** @param last_pid: Output parameter for last child PID (for exit status)
** @return: Number of processes created, or -1 on error
*/
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
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		curr = curr->next;
		i++;
	}
	*last_pid = pid[i - 1];
	return (i);
}

/*
** Main function to execute a pipeline of commands.
** Allocates PID array, creates pipes and forks for each command,
** waits for all children to complete, and sets exit status from last command.
**
** @param mini: Shell state containing commands to execute
** @return: 0 on success, 1 on error (malloc or pipe/fork failure)
*/
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
