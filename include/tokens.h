/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:50:47 by phofer            #+#    #+#             */
/*   Updated: 2026/02/09 17:31:05 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TOKENS_H
# define TOKENS_H

# include "minishell.h"
# include <stddef.h>
# include <stdint.h>

typedef enum e_token_type
{
	T_NONE,
	T_STRING,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND
}	t_token_type;

typedef struct s_word_range
{
	size_t	end;
	size_t	len;
}	t_word_range;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				heredoc;
	struct s_token	*next;
}	t_token;

#endif
