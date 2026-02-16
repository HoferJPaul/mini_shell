/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:19:14 by phofer            #+#    #+#             */
/*   Updated: 2026/02/16 14:34:50 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

//initiates parsing/tokenizing functions - located in ../src/parser
void	process_line(t_shell *mini, char *input)
{
	if (!tokenize_input(mini, input))
		return ;
	expand_tokens(mini);
	//parse_tokens();
}
//assuming we already initialized env and env pointer in mini struct already points at created env.
int setup_struct(t_shell *mini, char **envp)
{
	//char *path_value;

    if (!mini || !envp)
        return (0);

	//(void)envp;//we don't really need to get this as an argument if we already copied the envp to our copy.
    //mini->env = envp; //broken :(
    mini->flag = 0;
    mini->g_exit_status = 0;
    mini->running = 1;

	/*
    path_value = env_get(mini->env, "PATH");
    if (path_value)
        mini->paths = ft_split(path_value, ':');
    else
        mini->paths = NULL;
		//cleanup the initialized stuff(?)
    if (path_value && !mini->paths)
        return (0);
    mini->cwd = getcwd(NULL, 0);
    if (!mini->cwd)
        return (0);
		*/
    return (1);
}
