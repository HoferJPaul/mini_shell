/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:19:14 by phofer            #+#    #+#             */
/*   Updated: 2026/02/10 14:34:09 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

//initiates parsing/tokenizing functions - located in ../src/parser
void	process_line(t_shell *mini, char *input)
{
	if (!tokenize_input(mini, input))
		return ;
	// expand_tokens();
	// parse_tokens();
}
//assuming we already initialized env and env pointer in mini struct already points at created env.
int setup_struct(t_shell *mini, char **envp, t_env *env)
{
    if (!mini || !env)
        return (0);
	(void)envp;//we don't really need to get this as an argument if we already copied the envp to our copy.
    mini->env = env;
    mini->flag = 0;
    mini->g_exit_status = 0;
    mini->running = 1;
	init_paths(env, mini);
    mini->cwd = getcwd(NULL, 0);
    if (!mini->cwd)
        return (0);
    return (1);
}
