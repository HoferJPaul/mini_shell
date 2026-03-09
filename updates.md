update 04.12 1730 by phofer
{
	- created subfolders 'parser, utils'
	- added keywords to makefile
	- added header parser.h
	- removed unused functions from main.c
	- fixed compile issues with ft_isspace
	- created function process_line()
	- started on parsing/lexing functions

	#make compiles and it runs with working history!
}
update 05.02 14.30 by zgahrama
{
	-did the signal handling functions
	-there is also handling ctrl-d handling which was in main function before.
		**technically it is not a signal we are catching but all the ctrl features are in the same place now.
	-already tested, should compile without an issue.
}
update 05.02 17.06 by zgahrama
{
	-wrote the echo built-in function
	-tested in different environment since we dont have the parser yet.
	-first version, can update later on project
}
update 06.02 12.41 by zgahrama
{
	-wrote the env built-in function
	-tested but after the parser ready, needs re-testing
}
update 06.02 15.13 by zgahrama
{
	-wrote pwd function
	-tested but after the parser ready, needs re-testing
	-checked with valgrind as well shouldn't have leaks.
}
update 06.02 17:00 by phofer
{
	- updated main.c:
		- moved funtions into init.c
		- created shell_loop(), init()
	- created t_shell struct in minishell.h
	- created global variable g_sigint_received
	- changed setup_signals() to work with new global
}
update 08.02 16:00 by phofer
{
	- created various functions to handle parsing/lexing
	- created enum for token types
		(in parsing branch)
}
update 09.02 15.07 by zgahrama
{
	-environment copying
	-env functions like env_get and env_set done
	-checked in seperate directory, should work properly
}
update 08.02 18:00 by phofer
{
	- finished tokenization! (1/3 parts of paring ;D )
	- tested and everything works succesfully.
	- main compiles, but currently no way to interact/test without test functions
}

update 10.02 13.08 by zgahrama
{
	-added some variables to shell structure
	-updated the shell init function
	**on a side note: i think we don't need the envp argument,
	**we can just copy it to our env structure at the start of the programme and use it.
}

update 11.02 12.12 by zgahrama
{
	-path creating/refreshing functions created
	-unit testing
}

update 11.02 16.41 by zgahrama
{
	-replica of cd builtin implemented.
	-unit testing
	-update on some other built-ins like pwd, to work with the new structure.
	**side note: right now it takes the **argv as the argument but we can change it if it doesnt work with the structure
}

update 16.02 18:00 by phofer
{
	- finished tokenization and expansion!!!!
	- updated headers, added briefs
	- updated makefile
	- project compiles successfuly, recognizes unclosed quotes input
}

update 18.02 16.09 by zgahrama
{
	-unset builtin done
	-unit testing done
}
update 20.02 11.57 by zgahrama
{
	-exit builtin done.
	-passes tests.
	**note: handling it(if its called in parent or child will be in the execution part of the project)
}
update 25.02 16.57 by zgahrama
{
	-execution phase version 1.
	-created the execution directory and exec.c in it
	-connected the shell logic to builtins.
	-updated the builtin functions for working with tokens.
	-all builtins should work properly, tested with different scenarios.
	**exit not fully done yet, we need to decide how we will handle exit codes, rn only returns exit codes but not exiting anything.
}
update 27.02 17.22 by zgahrama
{
	-execution phase version 1.1.
	-created external commands directory.
	-succesfully runs the external commands on child process.
	**might need more testing.
}
update 02.03 15.27 by zgahrama
{
	-created exec_builtin file.
	-handling builtin execution with pipes.
	-execution function handles builtin called in pipes with child process.
	-updated some builtins to return int to give the exit status.
	-tested, should compile and work properly.
	**needs more testing after pipes are finished.
}
update 04.03 16:00 by phofer
{
	- added heredoc.c and redirect.c
	- fixed some meory leaks
	- added NULL Guards
	- did a bunch of testing and small fixes
	- heredoc pipes terminated with ctrl + C currenlty broken, I think we have to make a separate signal function for it
}
update 05.03 16.01 by zgahrama
{
	- ctrl + c bugfix
	- exit function now exits(still needs cleanup functions before exiting)
	- wrote cleanup for env struct.
}
update 06.03 16:00 by phofer
{
	- fixed stdin not being restored after failed redirect
	- fixed ctrl_d exiting with hardcoded 0 instead of g_exit_status
	- added free_tokens.c and hooked cleanup into shell_loop
	- fixed echo builtin using printf (buffered) instead of ft_putstr_fd
	- fixed execve exit code: ENOENT now exits 127 instead of 126 for missing path commands
	- fixed $EMPTY expansion: empty args are now skipped before execution, matching bash behaviour
}
update 09.03 16:00 by phofer
{
	- fixed some bugs, memory leaks
	- updated signal logic, seperate handlers for child/parent
	- started norm corections
}
