/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:53:14 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/10 16:08:18 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokens.h"

int	check_var_format(char *var)
{
	int	i;

	if (var[0] && (var[0] == '_' || ft_isalpha(var[0]) == 1))
	{
		i = 1;
		while (var[i] && var[i] != '=')
		{
			if (var[i] != '_' && ft_isalnum(var[i]) != 1)
			{
				ft_putstr_fd("export: not a valid identifier\n", 2);
				return (-1);
			}
			i++;
		}
		return (1);
	}
	ft_putstr_fd("export: not a valid identifier\n", 2);
	return (-1);
}

char	*parse_key(char *var)
{
	char	*key;
	int		i;
	int		len;

	if (check_var_format(var) == -1)
		return (NULL);
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	len = i;
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	i = 0;
	while (i < len)
	{
		key[i] = var[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*parse_val(char *var)
{
	char	*val;
	int		i;
	int		idx;
	int		val_len;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (!var[i])
		return (NULL);
	i++;
	val_len = ft_strlen(var + i);
	val = malloc(sizeof(char) * (val_len + 1));
	if (!val)
		return (NULL);
	idx = 0;
	while (var[i])
	{
		val[idx] = var[i];
		i++;
		idx++;
	}
	val[idx] = '\0';
	return (val);
}

// Parses and sets/exports one key=value arg; sets *status=1 on bad key.
static void	export_one(t_env **env, char *arg, int *status)
{
	char	*key;
	char	*val;

	key = parse_key(arg);
	if (!key)
	{
		*status = 1;
		return ;
	}
	val = parse_val(arg);
	if (!val)
		env_set(env, key, NULL, 1);
	else
		env_set(env, key, val, 1);
	free(key);
	free(val);
}

// Set or update an environment variable; if no value is given,
//	mark as exported.
int	export(t_env **env, char **command)
{
	int		i;
	int		status;

	if (!command[1])
	{
		print_env(*env);
		return (0);
	}
	i = 1;
	status = 0;
	while (command[i])
	{
		export_one(env, command[i], &status);
		++i;
	}
	return (status);
}
