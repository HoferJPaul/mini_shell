/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:46:59 by phofer            #+#    #+#             */
/*   Updated: 2026/03/04 16:27:20 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/parser.h"

/*
** Retrieves the value of a variable from the environment or special vars.
** Special variables:
** - $? : exit status from mini->g_exit_status
** - $0-$9 : positional parameters (return empty string)
** - Other: environment variables via getenv()
**
** @param name Variable name (without $)
** @param len  Length of variable name
** @param mini Shell structure containing exit status
** @return     Newly allocated string with var value, or empty string if unset
*/
static char	*get_var_value(const char *name, size_t len, t_shell *mini)
{
	char	*var_name;
	char	*value;
	char	*special;

	if (len == 1)
	{
		special = get_special_var(name[0], mini);
		if (special)
			return (special);
	}
	var_name = ft_substr(name, 0, len);
	if (!var_name)
		return (NULL);
	value = env_get(mini->env, var_name); /* use shell env, not process env */
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

/*
** Expands a variable reference ($VAR) and appends its value to the buffer.
** Handles edge cases:
** - Lone $ at end of string -> append literal $
** - Invalid variable name -> append literal $
** - Variable not found -> append empty string
**
** Updates the index i to point after the variable name.
**
** @param s    Input string
** @param i    Pointer to current index (at $)
** @param ctx  Buffer context
** @param mini Shell structure
** @return     1 on success, 0 on malloc failure
*/
static int	expand_var(const char *s, size_t *i, t_buf_ctx *ctx, t_shell *mini)
{
	size_t	var_len;
	char	*value;
	int		success;

	(*i)++;
	if (s[*i] == '\0')
		return (append_char(ctx, '$'));
	var_len = get_var_name_len(s, *i);
	if (var_len == 0)
		return (append_char(ctx, '$'));
	value = get_var_value(&s[*i], var_len, mini);
	if (!value)
		return (0);
	success = append_str(ctx, value);
	free(value);
	*i += var_len;
	return (success);
}

/*
** Processes text inside single quotes.
** Single quotes preserve everything literally - no expansion occurs.
** Removes the quotes themselves from output.
**
** @param s    Input string
** @param i    Pointer to current index (at opening ')
** @param ctx  Buffer context
** @return     1 on success, 0 on malloc failure
*/
static int	process_single_quotes(const char *s, size_t *i, t_buf_ctx *ctx)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
	{
		if (!append_char(ctx, s[*i]))
			return (0);
		(*i)++;
	}
	if (s[*i] == '\'')
		(*i)++;
	return (1);
}

/*
** Processes text inside double quotes.
** Double quotes allow variable expansion but remove the quotes.
** Variables are expanded, other text is preserved literally.
**
** @param s    Input string
** @param i    Pointer to current index (at opening ")
** @param ctx  Buffer context
** @param mini Shell structure
** @return     1 on success, 0 on malloc failure
*/
static int	process_double_quotes(const char *s, size_t *i,
				t_buf_ctx *ctx, t_shell *mini)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$')
		{
			if (!expand_var(s, i, ctx, mini))
				return (0);
		}
		else
		{
			if (!append_char(ctx, s[*i]))
				return (0);
			(*i)++;
		}
	}
	if (s[*i] == '"')
		(*i)++;
	return (1);
}

int	process_char(const char *s, size_t *i,
				t_buf_ctx *ctx, t_shell *mini)
{
	if (s[*i] == '\'')
		return (process_single_quotes(s, i, ctx));
	else if (s[*i] == '"')
		return (process_double_quotes(s, i, ctx, mini));
	else if (s[*i] == '$')
		return (expand_var(s, i, ctx, mini));
	else
	{
		if (!append_char(ctx, s[*i]))
			return (0);
		(*i)++;
		return (1);
	}
}
