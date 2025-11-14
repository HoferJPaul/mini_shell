#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int g_exit_status = 0;

static void sigint_handler(int signo)
{
	(void)signo;
	g_exit_status = (128 + SIGINT); //should be 130
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
int main(void)
{
	char *input;

	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input) {
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
			// TODO: Parse + execute
		free(input);
	}

	return (g_exit_status);
}
