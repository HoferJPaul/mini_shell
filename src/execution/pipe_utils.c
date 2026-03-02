/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/25 16:51:12 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/execute.h"

//Allocates pipe array
static int	allocate_pipes(t_exec_ctx *ctx, int pipe_count)
{
	int	i;

	ctx->pipes = malloc(sizeof(int *) * pipe_count);
	if (!ctx->pipes)
		return (1);
	i = 0;
	while (i < pipe_count)
	{
		ctx->pipes[i] = malloc(sizeof(int) * 2);
		if (!ctx->pipes[i])
		{
			while (--i >= 0)
				free(ctx->pipes[i]);
			free(ctx->pipes);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
** Sets up all pipes needed for pipeline
**
** @param ctx       Execution context
** @param cmd_count Number of commands
** @return          0 on success, 1 on error
*/
int	setup_pipes(t_exec_ctx *ctx, int cmd_count)
{
	int	i;

	ctx->pipe_count = cmd_count - 1;
	if (ctx->pipe_count <= 0)
	{
		ctx->pipes = NULL;
		ctx->pids = malloc(sizeof(pid_t) * cmd_count);
		return (ctx->pids == NULL);
	}
	if (allocate_pipes(ctx, ctx->pipe_count) != 0)
		return (1);
	i = 0;
	while (i < ctx->pipe_count)
	{
		if (pipe(ctx->pipes[i]) == -1)
		{
			cleanup_exec_ctx(ctx);
			return (1);
		}
		i++;
	}
	ctx->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!ctx->pids)
		return (cleanup_exec_ctx(ctx), 1);
	return (0);
}

/*
** Closes all pipe file descriptors in parent
*/
void	close_all_pipes(t_exec_ctx *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->pipe_count)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
}

/*
** Frees execution context
*/
void	cleanup_exec_ctx(t_exec_ctx *ctx)
{
	int	i;

	if (ctx->pipes)
	{
		i = 0;
		while (i < ctx->pipe_count)
		{
			free(ctx->pipes[i]);
			i++;
		}
		free(ctx->pipes);
		ctx->pipes = NULL;
	}
	if (ctx->pids)
	{
		free(ctx->pids);
		ctx->pids = NULL;
	}
}
