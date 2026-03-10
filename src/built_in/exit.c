/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:54:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:02:24 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

// checks the if the string is valid. returns 1 on error, 0 on success.
static int	check_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (1);
	while (ft_isdigit(str[i]))
		i++;
	if (str[i] != '\0')
		return (1);
	return (0);
}

// slightly updated atoi to handle long long type.
// now it returns 0 on success 1 on error.
// writes the result in pointer value
static int	safe_atoi(char *str, long long *out)
{
	int			i;
	int			sign;
	long long	result;
	int			digit;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (1);
		result = result * 10 + digit;
		i++;
	}
	*out = result * sign;
	return (0);
}

// Prints numeric error, frees shell and exits with code 2.
static void	exit_numeric_error(t_shell *mini, char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_dobby(mini);
	exit(2);
}

// Exits the shell with optional exit code.
// Validates numeric argument and handles errors.
// if running in pipeline → terminate only child.
// If running in parent → terminate entire shell.
int	ft_exit(t_shell *mini, char **command)
{
	long long	num;
	int			exit_stat;

	if (!command[1])
	{
		exit_stat = mini->g_exit_status;
		free_dobby(mini);
		exit(exit_stat);
	}
	if (check_str(command[1]) || safe_atoi(command[1], &num))
		exit_numeric_error(mini, command[1]);
	if (command[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		mini->g_exit_status = 1;
		return (mini->g_exit_status);
	}
	mini->g_exit_status = (unsigned char)num;
	exit_stat = mini->g_exit_status;
	free_dobby(mini);
	exit(exit_stat);
}
