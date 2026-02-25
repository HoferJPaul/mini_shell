/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:54:21 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/25 16:27:00 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"
//checks the if the string is valid. returns 1 on error, 0 on success.
static int  check_str(char *str)
{
    int i = 0;

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
//slightly updated atoi to handle long long type. now it returns 0 on success 1 on error. writes the result in pointer value
static int  safe_atoi(char *str, long long *out)
{
    int i = 0;
    int sign = 1;
    long long result = 0;
    int digit;

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

//if running in pipeline → terminate only child.
//If running in parent → terminate entire shell.

// Exits the shell with optional exit code. Validates numeric argument and handles errors.
int ft_exit(t_shell *mini, t_token *tokens)
{
    long long num;
    t_token *curr;
    
    curr = tokens;
    if (!curr)
        return (EXIT_SIGNAL);
    if (curr->next)
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        mini->g_exit_status = 1;
        return (1);
    }
    if (check_str(curr->value) || safe_atoi(curr->value, &num))
    {
        ft_putstr_fd("exit: ", 2);
        ft_putstr_fd(curr->value, 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        mini->g_exit_status = 255;
        return (EXIT_SIGNAL);
    }
    mini->g_exit_status = (unsigned char)num;
    return (EXIT_SIGNAL);
}

