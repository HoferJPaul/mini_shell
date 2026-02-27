/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:34:28 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/27 17:18:17 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

static int	check_builtins(t_shell *mini)
{
	t_token	*curr;

	curr = mini->tokens;
	if (ft_strncmp(curr->value, "echo", ft_strlen(curr->value)) == 0)
	{
		curr = curr->next;
		echo(curr); //" character doesn't get removed in tokenization.
		return (0);
	}
	else if (ft_strncmp(curr->value, "env", ft_strlen(curr->value)) == 0)
	{
		env(mini->env);
		return (0);
	}
	else if (ft_strncmp(curr->value, "pwd", ft_strlen(curr->value)) == 0)
	{
		pwd(mini);
		return (0);
	}
	else if (ft_strncmp(curr->value, "cd", ft_strlen(curr->value)) == 0)
	{
		curr = curr->next;
		cd(mini, curr);
		return (0);
	}
	else if (ft_strncmp(curr->value, "export", ft_strlen(curr->value)) == 0)
	{
		curr = curr->next;
		export(&mini->env, curr);
		return (0);
	}
	else if (ft_strncmp(curr->value, "unset", ft_strlen(curr->value)) == 0)
	{
		curr = curr->next;
		unset(&mini->env, curr);
		return (0);
	}
	else if (ft_strncmp(curr->value, "exit", ft_strlen(curr->value)) == 0)
	{
		curr = curr->next;
		ft_exit(mini, curr);
		return (0);
	}
	return (1);
}

/*
** Executes one command.
**
** 1. If the command is a builtin that must run in the parent
**    (like cd, export, exit), execute it directly.
**
** 2. Otherwise fork a child process:
**    - Child executes the external command with execve().
**    - Parent waits for the child using waitpid().
**
** 3. After waitpid(), the raw status is translated into the
**    shell exit status:
**       - Normal exit  -> exit code
**       - Signal death -> 128 + signal number
**
** The final result is stored in mini->g_exit_status ($?).
*/
int execution(t_shell *mini)
{
    pid_t p;
    int status;

    if (check_builtins(mini) == 0)
        return (0);
    p = fork();
    if (p < 0)
    {
        perror("fork");
        return (1);
    }
    if (p == 0)
    {
        exec_external(mini, mini->tokens);
        exit(1);//backup exit
    }
    waitpid(p, &status, 0);
    if (WIFEXITED(status))
        mini->g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        mini->g_exit_status = 128 + WTERMSIG(status);
    return (0);
}
