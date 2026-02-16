/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 11:57:41 by zgahrama          #+#    #+#             */
/*   Updated: 2026/02/16 15:28:08 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
/* ===================== C STANDARD ===================== */

#include <stdlib.h> // malloc, free, getenv, exit
#include <stdio.h>  // printf, perror
#include <string.h> // strerror
#include <errno.h>  // errno

/* ===================== UNIX / POSIX ==================== */

#include <unistd.h> // read, write, access, close, fork, execve,
                    // dup, dup2, pipe, chdir, getcwd,
                    // isatty, ttyname, ttyslot

#include <fcntl.h>     // open
#include <sys/types.h> // pid_t
#include <sys/stat.h>  // stat, lstat, fstat, unlink
#include <dirent.h>    // opendir, readdir, closedir

/* ===================== PROCESS / WAIT ================== */

#include <sys/wait.h> // wait, waitpid, wait3, wait4

/* ===================== SIGNALS ========================= */

#include <signal.h> // signal, sigaction,
                    // sigemptyset, sigaddset, kill

/* ===================== TERMINAL / TTY ================== */

#include <termios.h>   // tcgetattr, tcsetattr
#include <sys/ioctl.h> // ioctl

/* ===================== TERMCAP ========================= */

#include <termcap.h> // tgetent, tgetflag, tgetnum,
                     // tgetstr, tgoto, tputs

/* ===================== READLINE ======================== */

#include <readline/readline.h> // readline, rl_on_new_line,
                               // rl_replace_line, rl_redisplay
#include <readline/history.h>  // add_history, rl_clear_history

// =================== STRUCT ===============================
typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

extern volatile sig_atomic_t g_sigint_received;
typedef struct s_token	t_token;

// =================== STRUCT ===============================

typedef struct s_shell
{
    struct s_env	*env;				//copied env pointer
    char 			**paths;			// Parsed PATH directories (from env)
    char 			*cwd;				// Current working directory
    int 			flag;
    int 			g_exit_status;
    int 			running;
	t_token *tokens;
} t_shell;

// =================== FUNCTIONS ============================
void	process_line(t_shell *mini, char *input);
int setup_struct(t_shell *mini, char **envp);
void sigint_handler(int signo);
void setup_signals(t_shell *mini);
void ctrl_d(char *line);
//================ ENV CREATION AND UTILS ===================
t_env *create_env(char *key, char *value);
t_env *env_from_string(char *str);
t_env *env_copy(char **envp);
char *env_get(t_env *env, char *key);
void env_set(t_env **env, char *key, char *value);
int env_size(t_env *env);
char **env_to_array(t_env *env);
void init_paths(t_env *env, t_shell *mini);
void refresh_paths(t_shell *mini);
void add_new_paths(t_shell *mini, char *new_path);

#endif
