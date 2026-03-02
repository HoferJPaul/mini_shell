/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/02 17:36:43 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/command.h"
#include "../include/execute.h"

/*
** Closes all pipes in child process
*/
static void	close_unused_pipes(t_exec_ctx *ctx, int keep_read, int keep_write)
{
	int	i;

	i = 0;
	while (i < ctx->pipe_count)
	{
		if (i != keep_read)
			close(ctx->pipes[i][0]);
		if (i != keep_write)
			close(ctx->pipes[i][1]);
		i++;
	}
}

/*
** Sets up pipe file descriptors for a command in the pipeline
**
** @param ctx        Execution context
** @param cmd_idx    Command index (0-based)
** @param total_cmds Total number of commands
** @return           0 on success, 1 on error
*/
int	setup_pipe_fds(t_exec_ctx *ctx, int cmd_idx, int total_cmds)
{
	if (cmd_idx > 0)
	{
		if (dup2(ctx->pipes[cmd_idx - 1][0], STDIN_FILENO) == -1)
			return (1);
	}
	if (cmd_idx < total_cmds - 1)
	{
		if (dup2(ctx->pipes[cmd_idx][1], STDOUT_FILENO) == -1)
			return (1);
	}
	close_unused_pipes(ctx, -1, -1);
	return (0);
}

/*
** Child process execution
*/
static void	child_process(t_shell *mini, t_command *cmd,
				t_exec_ctx *ctx, int idx)
{
	char	*path;
	int		total_cmds;
	char	**env_arr;

	total_cmds = ctx->pipe_count + 1;
	if (setup_pipe_fds(ctx, idx, total_cmds) != 0)
		exit(1);
	// if (setup_redirects(cmd->redirects) != 0) //TODO!
	// 	exit(1);
	if (check_builtin(cmd->args[0])) //merge
		exit(execute_builtin(mini, cmd)); //merge
	env_arr = env_to_array(mini->env); //possible duplicate?
	path = find_command_path(cmd->args[0], env_arr); //merge
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, env_arr);
	ft_putstr_fd("execve failed\n", 2);
	free(path);
	free(env_arr);
	exit(1);
}

/*
** Executes a single command in the pipeline
**
** @param mini Shell structure
** @param cmd  Command to execute
** @param ctx  Execution context
** @param idx  Command index in pipeline
** @return     0 on success, 1 on error
*/
int	execute_command(t_shell *mini, t_command *cmd, t_exec_ctx *ctx, int idx)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
		return (0);
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork failed\n", 2);
		return (1);
	}
	if (pid == 0)
		child_process(mini, cmd, ctx, idx);
	ctx->pids[idx] = pid;
	return (0);
}
