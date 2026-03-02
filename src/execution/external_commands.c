/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:11:00 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/02 19:40:35 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

char	*find_command_path(t_shell *mini, char *cmd)
{
	char	*path;
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	path = env_get(mini->env, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}
/*we dont need this anymore with the new structure
char	**tokens_to_argv(t_token *tokens)
{
	char	**argv;
	int		count;
	int		i;
	t_token	*tmp;

	count = 0;
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	while (tokens)
	{
		argv[i++] = tokens->value;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}*/
// just executing it as it is, doesn't check if theres a pipe,
//	assumes already being called in the child process
void	exec_external(t_shell *mini, char **command)
{
	char	*path;

	if (ft_strchr(command[0], '/')) // for running with path value
		path = command[0];
	else
		path = find_command_path(mini, command[0]);
	if (!path)
	{
		printf("%s: command not found\n", command[0]);
		exit(127);
	}
	execve(path, command, env_to_array(mini->env));
	perror("execve");
	exit(1);
}
