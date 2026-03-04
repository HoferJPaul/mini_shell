/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phofer <phofer@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 11:57:41 by zgahrama          #+#    #+#             */
/*   Updated: 2026/03/04 17:23:26 by phofer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "command.h"

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
#include <limits.h>
// =================== STRUCT ===============================
#define EXIT_SIGNAL -42
typedef struct s_env
{
    char *key;
    char *value;
    int   exported_flag;
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
    int             has_pipe;//set this in parsing?
	t_command	*commands;
	t_token *tokens;
} t_shell;

// =================== FUNCTIONS ============================
void	process_line(t_shell *mini, char *input);
int setup_struct(t_shell *mini, t_env **env);
void sigint_handler(int signo);
void	heredoc_sigint(int sig);
void setup_signals(t_shell *mini);
void ctrl_d(char *line);
// =================== EXECUTION =====================
int execution(t_shell *mini);
void exec_external(t_shell *mini, char **command);
//int exec_builtin_child(t_shell *mini, t_command *command);
int	check_builtin(char *cmd);
int exec_builtin(t_shell *mini, char **command);
int	execution_pipe(t_shell *mini);
int	apply_redirections(t_command *cmd);
//================= BUILTINS =================================
int export(t_env **env, char **command);
int cd(t_shell *mini, char **command);
int echo(char **command);
int env(t_env *env);
int ft_exit(t_shell *mini, char **command);
int pwd(t_shell *mini);
int unset(t_env **env, char **command);
//================ ENV CREATION AND UTILS ===================
t_env *create_env(char *key, char *value, int exported);
t_env *env_from_string(char *str);
t_env *env_copy(char **envp);
char *env_get(t_env *env, char *key);
void env_set(t_env **env, char *key, char *value, int exported_flag);
int env_size(t_env *env);
char **env_to_array(t_env *env);
void init_paths(t_env *env, t_shell *mini);
void refresh_paths(t_shell *mini);
void add_new_paths(t_shell *mini, char *new_path);
void print_env(t_env *env);
int	collect_heredocs(t_shell *mini);

//==================CLEANUP===================================
void free_array(char **arr);

//================TESTING================================
void	print_tokens_debug(t_token *tokens);
void print_commands_shell_format(t_command *commands);

#endif
