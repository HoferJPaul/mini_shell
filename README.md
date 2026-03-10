# Minishell

*This project has been created as part of the 42 curriculum by phofer, zgahrama.*

## Description

**Minishell** is a simplified Unix shell written in C as part of the 42 School curriculum. The objective of the project is to recreate a minimal version of Bash that can interpret and execute user commands in an interactive environment.

The project focuses on understanding how shells work internally, particularly in relation to:

- Process creation (`fork`)
- Program execution (`execve`)
- File descriptor management
- Pipes and inter-process communication
- Signal handling
- Environment variable management

The shell displays a prompt, reads commands from the user, parses them, and executes them while supporting core shell features such as pipes, redirections, environment variables, and built-in commands.

### Mandatory Features

Minishell implements the following functionality:

- Interactive prompt
- Command history (via `readline`)
- Execution of binaries using `PATH`
- Built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Pipes (`|`)
- Redirections
  - `<` input redirection
  - `>` output redirection
  - `>>` append redirection
  - `<<` heredoc
- Environment variable expansion (`$VAR`)
- Exit status expansion (`$?`)
- Quote handling
  - Single quotes `'`
  - Double quotes `"`
- Signal handling
  - `Ctrl+C`
  - `Ctrl+D`
  - `Ctrl+\`

The shell aims to behave similarly to Bash for the supported subset of features.

### Bonus Features (if implemented)

- Logical operators:
  - `&&`
  - `||`
- Parentheses for priority grouping
- Wildcard expansion (`*`)

---

# Instructions

## Compilation

Clone the repository and compile the project using:

```bash
make
```

This will produce the executable:

```
minishell
```

### Makefile Rules

| Rule | Description |
|-----|-------------|
| `make` or `make all` | Compile the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executable |
| `make re` | Recompile the project |

The project is compiled using the flags:

```
-Wall -Wextra -Werror
```

---

## Running Minishell

Start the shell by executing:

```bash
./minishell
```

You will see the shell prompt:

```
minishell>
```

Example usage:

```bash
minishell> echo Hello World
Hello World

minishell> ls -l | grep .c

minishell> cat file.txt > output.txt

minishell> echo $HOME
/home/user
```

Exit the shell using:

```bash
exit
```

or by pressing:

```
Ctrl+D
```

---

# Implementation Overview

The project is typically structured into several modules:

### 1. Input Handling
Uses the **GNU Readline library** to display the prompt and manage command history.

### 2. Tokenization
The input string is split into tokens such as:

- words
- pipes
- redirections
- operators

### 3. Expansion
Handles:

- environment variables (`$VAR`)
- exit status (`$?`)
- quote rules

### 4. Parsing
Tokens are organized into command structures representing pipelines and redirections.

### 5. Execution
Commands are executed by:

- creating processes (`fork`)
- setting up pipes (`pipe`)
- redirecting file descriptors (`dup2`)
- launching executables (`execve`)

### 6. Built-ins
Some commands are executed directly by the shell without spawning a new process.

---

# Resources

### Documentation

- 42 Minishell Subject
- Bash Reference Manual
  https://www.gnu.org/software/bash/manual/

- POSIX Shell Documentation
  https://pubs.opengroup.org/onlinepubs/9699919799/

- Linux man pages
  https://man7.org/linux/man-pages/

### Helpful Guides

- 42 Minishell Guide
  https://42-cursus.gitbook.io/guide/3-rank-03/minishell

---

# AI Usage

AI tools were used only as a development aid and learning resource.

AI was used for:

- Explaining low‑level system calls (`fork`, `execve`, `dup2`, `pipe`)
- Understanding parsing and shell architecture
- Debugging complex issues related to:
  - pipes
  - heredocs
  - signal handling
- Assisting with documentation generation

All generated suggestions were reviewed, tested, and modified where necessary to ensure full understanding and compliance with the 42 project rules.

---

# Authors

- phofer
- zgahrama
