/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:35:19 by phofer            #+#    #+#             */
/*   Updated: 2026/02/25 16:51:32 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/execute.h"
#include "../include/command.h"

//Waits for all child processes and gets exit status
static int	wait_for_children(t_exec_ctx *ctx, int cmd_count)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (i < cmd_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (exit_status);
}

//Executes each command in the pipeline
static int	execute_all_commands(t_shell *mini, t_command *commands,
				t_exec_ctx *ctx, int cmd_count)
{
	t_command	*current;
	int			i;

	current = commands;
	i = 0;
	while (current && i < cmd_count)
	{
		if (execute_command(mini, current, ctx, i) != 0)
			return (1);
		current = current->next;
		i++;
	}
	return (0);
}

/*
** Counts commands in pipeline
*/
int	count_commands(t_command *commands)
{
	int			count;
	t_command	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
** Main pipeline execution function
** Sets up pipes, forks processes, waits for completion
**
** @param mini     Shell structure
** @param commands Command list
** @return         0 on success, 1 on error
*/

int	execute_pipeline(t_shell *mini, t_command *commands)
{
	t_exec_ctx	ctx;
	int			cmd_count;
	int			exit_status;

	cmd_count = count_commands(commands);
	ft_memset(&ctx, 0, sizeof(t_exec_ctx));
	if (setup_pipes(&ctx, cmd_count) != 0)
		return (1);
	if (execute_all_commands(mini, commands, &ctx, cmd_count) != 0)
	{
		cleanup_exec_ctx(&ctx);
		return (1);
	}
	close_all_pipes(&ctx);
	exit_status = wait_for_children(&ctx, cmd_count);
	cleanup_exec_ctx(&ctx);
	mini->g_exit_status = exit_status;
	return (0);
}
