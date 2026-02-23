/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by phofer            #+#    #+#             */
/*   Updated: 2026/02/23 17:19:20 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "tokens.h"

/* Color codes for terminal output */
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define MAGENTA "\033[0;35m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

/*
** Token printing functions
*/
void	print_tokens(t_token *tokens);
void	print_tokens_compact(t_token *tokens);
void	print_tokens_pipeline(t_token *tokens);
void	print_tokens_debug(t_token *tokens);
void	print_single_token(t_token *token, int index, int show_addr);

/*
** Command printing functions
*/
void	print_commands(t_command *commands);
void	print_commands_compact(t_command *commands);
void	print_commands_pipeline(t_command *commands);
void	print_commands_debug(t_command *commands);
void	print_commands_shell_format(t_command *commands);
void	print_commands_debug(t_command *commands);
void	process_line_debug(t_shell *mini, char *input);

#endif
