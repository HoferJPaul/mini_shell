# Minishell Development Log

---

## 2024-12-04 17:30 — phofer
- Created subfolders: `parser`, `utils`
- Added keywords to Makefile
- Added header `parser.h`
- Removed unused functions from `main.c`
- Fixed compile issues with `ft_isspace`
- Created function `process_line()`
- Started parsing/lexing functions

**Status:**  
`make` compiles and program runs with working history.

---

## 2025-02-05 14:30 — zgahrama
- Implemented signal handling functions
- Added `Ctrl+D` handling (previously in `main`)
  - Not technically a signal, but grouped with control features
- Tested and compiles without issues

---

## 2025-02-05 17:06 — zgahrama
- Implemented `echo` builtin
- Tested independently (no parser yet)
- First version (subject to later updates)

---

## 2025-02-06 12:41 — zgahrama
- Implemented `env` builtin
- Tested
- Requires re-testing after parser integration

---

## 2025-02-06 15:13 — zgahrama
- Implemented `pwd` builtin
- Tested
- Requires re-testing after parser integration
- Checked with Valgrind (no leaks)

---

## 2025-02-06 17:00 — phofer
- Updated `main.c`:
  - Moved functions to `init.c`
  - Created `shell_loop()` and `init()`
- Created `t_shell` struct in `minishell.h`
- Added global variable `g_sigint_received`
- Updated `setup_signals()` to use global

---

## 2025-02-08 16:00 — phofer
- Created parsing/lexing functions
- Added enum for token types  
  *(in parsing branch)*

---

## 2025-02-08 18:00 — phofer
- Finished tokenization (1/3 of parsing)
- Fully tested and working
- Project compiles
- No interactive testing yet (needs test functions)

---

## 2025-02-09 15:07 — zgahrama
- Implemented environment copying
- Added `env_get` and `env_set`
- Tested in separate directory

---

## 2025-02-10 13:08 — zgahrama
- Added variables to `t_shell`
- Updated shell initialization

**Note:**  
`envp` argument may not be needed—copy once into internal structure.

---

## 2025-02-11 12:12 — zgahrama
- Implemented path creation/refresh functions
- Unit testing completed

---

## 2025-02-11 16:41 — zgahrama
- Implemented `cd` builtin
- Unit testing completed
- Updated builtins (e.g. `pwd`) for new structure

**Note:**  
Currently takes `**argv`; may need adjustment later.

---

## 2025-02-16 18:00 — phofer
- Finished tokenization + expansion
- Updated headers and added documentation
- Updated Makefile
- Handles unclosed quotes correctly

---

## 2025-02-18 16:09 — zgahrama
- Implemented `unset` builtin
- Unit testing completed

---

## 2025-02-20 11:57 — zgahrama
- Implemented `exit` builtin
- Passes tests

**Note:**  
Parent vs child handling will be done in execution phase.

---

## 2025-02-25 16:57 — zgahrama
- Execution phase v1:
  - Created `execution/exec.c`
  - Connected shell logic to builtins
  - Updated builtins to work with tokens
- All builtins tested and working

**Note:**  
`exit` not fully implemented (returns code but does not exit).

---

## 2025-02-27 17:22 — zgahrama
- Execution phase v1.1:
  - Created external commands module
  - External commands run in child processes

**Note:**  
Needs further testing.

---

## 2025-03-02 15:27 — zgahrama
- Created `exec_builtin.c`
- Implemented builtin execution with pipes
- Execution handles builtins in pipelines (child process)
- Updated builtins to return exit status

**Status:**  
Compiles and works  
**Note:** Needs more testing after full pipe implementation

---

## 2025-03-04 16:00 — phofer
- Added `heredoc.c` and `redirect.c`
- Fixed memory leaks
- Added NULL guards
- Extensive testing and fixes

**Issue:**  
`Ctrl+C` in heredoc breaks pipe termination → likely needs dedicated signal handler

---

## 2025-03-05 16:01 — zgahrama
- Fixed `Ctrl+C` bug
- `exit` now properly exits (cleanup still needed)
- Added cleanup for environment struct

---

## 2025-03-06 16:00 — phofer
- Fixed stdin restoration after failed redirect
- Fixed `Ctrl+D` returning `0` instead of `g_exit_status`
- Added `free_tokens.c` and integrated cleanup into `shell_loop`
- Fixed `echo` (used `printf` → replaced with `ft_putstr_fd`)
- Fixed `execve` exit codes:
  - `ENOENT` → `127` (was incorrectly `126`)
- Fixed `$EMPTY` expansion:
  - Empty args skipped before execution (matches Bash)

---

## 2025-03-09 16:00 — phofer
- Fixed bugs and memory leaks
- Updated signal logic:
  - Separate handlers for parent/child
- Started Norminette corrections

---

## 2025-03-11 16:00 — phofer
- Full day of Norminette corrections
