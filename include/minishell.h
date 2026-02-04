/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgahrama <zgahrama@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 11:57:41 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/04 12:03:47 by zgahrama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
/* ===================== C STANDARD ===================== */

# include <stdlib.h>     // malloc, free, getenv, exit
# include <stdio.h>      // printf, perror
# include <string.h>    // strerror
# include <errno.h>     // errno

/* ===================== UNIX / POSIX ==================== */

# include <unistd.h>     // read, write, access, close, fork, execve,
                         // dup, dup2, pipe, chdir, getcwd,
                         // isatty, ttyname, ttyslot

# include <fcntl.h>      // open
# include <sys/types.h>  // pid_t
# include <sys/stat.h>   // stat, lstat, fstat, unlink
# include <dirent.h>     // opendir, readdir, closedir

/* ===================== PROCESS / WAIT ================== */

# include <sys/wait.h>   // wait, waitpid, wait3, wait4

/* ===================== SIGNALS ========================= */

# include <signal.h>     // signal, sigaction,
                         // sigemptyset, sigaddset, kill

/* ===================== TERMINAL / TTY ================== */

# include <termios.h>    // tcgetattr, tcsetattr
# include <sys/ioctl.h>  // ioctl

/* ===================== TERMCAP ========================= */

# include <termcap.h>    // tgetent, tgetflag, tgetnum,
                         // tgetstr, tgoto, tputs

/* ===================== READLINE ======================== */

# include <readline/readline.h>   // readline, rl_on_new_line,
                                  // rl_replace_line, rl_redisplay
# include <readline/history.h>    // add_history, rl_clear_history


// =================== FUNCTIONS ============================

#endif
