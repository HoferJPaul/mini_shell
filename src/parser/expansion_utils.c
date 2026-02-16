/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:58:59 by phofer            #+#    #+#             */
/*   Updated: 2026/02/16 15:03:38 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/tokens.h"
#include "../include/parser.h"

int	realloc_buffer(t_buf_ctx *ctx)
{
	char	*new_buf;
	size_t	new_cap;

	new_cap = ctx->cap * 2;
	new_buf = malloc(new_cap);
	if (!new_buf)
		return (0);
	ft_memcpy(new_buf, ctx->buf, ctx->len);
	free(ctx->buf);
	ctx->buf = new_buf;
	ctx->cap = new_cap;
	return (1);
}

int	append_char(t_buf_ctx *ctx, char c)
{
	if (ctx->len >= ctx->cap - 1)
	{
		if (!realloc_buffer(ctx))
			return (0);
	}
	ctx->buf[ctx->len++] = c;
	return (1);
}

int	append_str(t_buf_ctx *ctx, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!append_char(ctx, str[i]))
			return (0);
		i++;
	}
	return (1);
}

size_t	get_var_name_len(const char *s, size_t i)
{
	size_t	len;

	if (s[i] == '?')
		return (1);
	if (ft_isdigit(s[i]))
		return (1);
	len = 0;
	while (s[i + len] && (ft_isalnum(s[i + len]) || s[i + len] == '_'))
		len++;
	return (len);
}

char	*get_special_var(char name, t_shell *mini)
{
	if (name == '?')
		return (ft_itoa(mini->g_exit_status));
	if (ft_isdigit(name))
		return (ft_strdup(""));
	return (NULL);
}
