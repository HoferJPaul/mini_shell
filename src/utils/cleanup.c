/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:09:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:43:39 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// should free the add_history with clear_history here
// use it for mini->paths
void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_env_nodes(t_env *env)
{
	t_env	*curr;
	t_env	*next;

	curr = env;
	while (curr != NULL)
	{
		next = curr->next;
		free(curr->key);
		if (curr->value)
			free(curr->value);
		free(curr);
		curr = next;
	}
}

// Frees mini->commands and mini->tokens, sets both to NULL.
static void	free_cmds_and_tokens(t_shell *mini)
{
	if (mini->commands)
	{
		free_commands(mini->commands);
		mini->commands = NULL;
	}
	if (mini->tokens)
	{
		free_tokens(mini->tokens);
		mini->tokens = NULL;
	}
}

// Master cleanup: frees all shell state fields and readline history.
void	free_dobby(t_shell *mini)
{
	if (!mini)
		return ;
	rl_clear_history();
	if (mini->cwd)
	{
		free(mini->cwd);
		mini->cwd = NULL;
	}
	if (mini->paths)
	{
		free_array(mini->paths);
		mini->paths = NULL;
	}
	if (mini->env)
	{
		free_env_nodes(mini->env);
		mini->env = NULL;
	}
	free_cmds_and_tokens(mini);
}

// Frees current input string and per-loop shell state (commands, tokens).
void	free_loop(t_shell *mini, char *input)
{
	if (input)
		free(input);
	if (!mini)
		return ;
	free_cmds_and_tokens(mini);
}
