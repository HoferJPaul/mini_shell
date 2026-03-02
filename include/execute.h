/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/03/02 17:30:35 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "command.h"

/*
** Execution context for pipeline
*/
typedef struct s_exec_ctx
{
	int		pipe_count;
	int		**pipes;
	pid_t	*pids;
}	t_exec_ctx;

/*
** Main execution function
*/
int		execute(t_shell *mini);

/*
** Pipeline execution
*/
int		execute_pipeline(t_shell *mini, t_command *commands);
int		setup_pipes(t_exec_ctx *ctx, int cmd_count);
void	close_all_pipes(t_exec_ctx *ctx);
void	cleanup_exec_ctx(t_exec_ctx *ctx);

/*
** Single command execution
*/
int		execute_command(t_shell *mini, t_command *cmd, t_exec_ctx *ctx, int idx);
int		setup_redirects(t_redirect *redirects);
int		setup_pipe_fds(t_exec_ctx *ctx, int cmd_idx, int total_cmds);

/*
** Built-in commands
*/
int		is_builtin(char *cmd);
int		execute_builtin(t_shell *mini, t_command *cmd);

/*
** Heredoc handling
*/
int		process_heredocs(t_command *commands);
int		read_heredoc(t_redirect *redir, t_shell *mini);

/*
** Path resolution
*/
char	*find_command_path(char *cmd, char **paths);

#endif
