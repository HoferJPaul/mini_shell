/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:19:14 by phofer            #+#    #+#             */
/*   Updated: 2026/03/05 17:57:04 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"
#include "../include/tokens.h"

//initiates parsing/tokenizing functions - located in ../src/parser
void	process_line(t_shell *mini, char *input)
{
	int	err;

	err = tokenize(mini, input);
	if (err)
		return ((void)(mini->g_exit_status = err));
	err = expand(mini);
	if (err)
		return ((void)(mini->g_exit_status = err));
	err = parse(mini);
	if (err)
		return ((void)(mini->g_exit_status = err));
	err = collect_heredocs(mini);
	if (err)
		return ;
}

int	setup_struct(t_shell *mini, t_env **env)
{
	char	*path_value;

	if (!mini || !env)
		return (0);
	mini->env = *env;
	mini->flag = 0;
	mini->g_exit_status = 0;
	mini->running = 1;
	path_value = env_get(mini->env, "PATH");
	init_paths(*env, mini);
	if (path_value && !mini->paths)
		return (0);
	mini->cwd = getcwd(NULL, 0);
	if (!mini->cwd)
	{
		free_array(mini->paths);
		mini->paths = NULL;
		return (0);
	}
	return (1);
}
