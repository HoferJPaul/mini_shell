#!/bin/bash
# =============================================================================
#  Minishell Mandatory Tester (42-subject aligned)
#  Usage: ./test_minishell_rewritten.sh [path/to/minishell]
#
#  Notes:
#  - avoids ';' because it is not required in the mandatory part
#  - captures minishell exit status correctly (without losing it in a pipeline)
#  - strips interactive prompts from stdout for fair comparison
#  - keeps some stderr checks as warnings when wording may differ
# =============================================================================

MINI="${1:-./minishell}"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

PASS=0; FAIL=0; WARN=0; TOTAL=0

TDIR="$(pwd)/.tester_tmp"
OUTFILES="$TDIR/outfiles"
TEST_FILES="$TDIR/test_files"

setup() {
    rm -rf "$TDIR"
    mkdir -p "$OUTFILES" "$TEST_FILES"
    printf "hello\nworld\nfoo\nbar\n" > "$TEST_FILES/infile"
    for i in $(seq 1 100); do echo "line $i oi"; done > "$TEST_FILES/infile_big"
    printf "content of spaced file\n" > "$TEST_FILES/file name with spaces"
    touch "$TEST_FILES/invalid_permission"
    chmod 000 "$TEST_FILES/invalid_permission"
}

teardown() {
    chmod 644 "$TEST_FILES/invalid_permission" 2>/dev/null
    rm -rf "$TDIR"
}

strip_prompts() {
    sed \
        -e '/^minishell> /d' \
        -e '/^> /d' \
        -e '/^exit$/d'
}

strip_bash_prefix() {
    sed 's/^bash: line [0-9]*: //' | sed 's/^bash: //'
}

run_mini() {
    local input="$1"
    printf '%s\n' "$input" > "$TDIR/input.txt"
    "$MINI" < "$TDIR/input.txt" > "$TDIR/mini_out_raw" 2> "$TDIR/mini_err"
    MINI_EXIT=$?
    MINI_STDOUT=$(strip_prompts < "$TDIR/mini_out_raw")
    MINI_STDERR=$(cat "$TDIR/mini_err")
}

run_bash() {
    local input="$1"
    printf '%s\n' "$input" > "$TDIR/input.txt"
    bash --norc --noprofile < "$TDIR/input.txt" > "$TDIR/bash_out_raw" 2> "$TDIR/bash_err"
    BASH_EXIT=$?
    BASH_STDOUT=$(cat "$TDIR/bash_out_raw")
    BASH_STDERR=$(strip_bash_prefix < "$TDIR/bash_err")
}

print_result() {
    local ok="$1" warn="$2" label="$3"
    shift 3
    local reasons=("$@")
    local num; printf -v num "%3d" "$TOTAL"

    if [[ "$ok" -eq 1 && "$warn" -eq 0 ]]; then
        echo -e "${GREEN}Test $num: ✅${RESET}  $label"
        PASS=$((PASS + 1))
    elif [[ "$ok" -eq 1 ]]; then
        echo -e "${YELLOW}Test $num: ✅⚠️ ${RESET}  $label"
        for r in "${reasons[@]}"; do
            [[ -n "$r" ]] && echo -e "         ${YELLOW}⚠ $r${RESET}"
        done
        PASS=$((PASS + 1))
        WARN=$((WARN + 1))
    else
        echo -e "${RED}Test $num: ❌${RESET}  $label"
        for r in "${reasons[@]}"; do
            [[ -n "$r" ]] && echo -e "         ${RED}→ $r${RESET}"
        done
        FAIL=$((FAIL + 1))
    fi
}

# run_test <label> <input>
# Optional flags (unset after each call):
#   WARN_STDERR=1   stderr mismatch is warning not failure
#   WARN_EXIT=1     exit mismatch is warning not failure
#   SKIP_STDERR=1   ignore stderr entirely
run_test() {
    local label="$1" input="$2"
    TOTAL=$((TOTAL + 1))
    rm -f "$OUTFILES"/*

    run_mini "$input"
    run_bash "$input"

    local ok=1 warn=0 reasons=()

    if [[ "$MINI_STDOUT" != "$BASH_STDOUT" ]]; then
        ok=0
        reasons+=("stdout  mini=[$MINI_STDOUT]  bash=[$BASH_STDOUT]")
    fi

    if [[ "$MINI_EXIT" != "$BASH_EXIT" ]]; then
        if [[ "${WARN_EXIT:-0}" == "1" ]]; then
            warn=1
            reasons+=("exit  mini=$MINI_EXIT  bash=$BASH_EXIT")
        else
            ok=0
            reasons+=("exit  mini=$MINI_EXIT  bash=$BASH_EXIT")
        fi
    fi

    if [[ "${SKIP_STDERR:-0}" != "1" && "$MINI_STDERR" != "$BASH_STDERR" ]]; then
        if [[ "${WARN_STDERR:-0}" == "1" ]]; then
            warn=1
            reasons+=("stderr  mini=[$MINI_STDERR]  bash=[$BASH_STDERR]")
        else
            ok=0
            reasons+=("stderr  mini=[$MINI_STDERR]  bash=[$BASH_STDERR]")
        fi
    fi

    unset WARN_STDERR WARN_EXIT SKIP_STDERR
    print_result "$ok" "$warn" "$label" "${reasons[@]}"
}

# run_seq <label> <expected_stdout> <expected_exit> cmd1 cmd2 ...
# Sends newline-separated commands to minishell (no semicolons required)
run_seq() {
    local label="$1" exp_out="$2" exp_exit="$3"
    shift 3
    TOTAL=$((TOTAL + 1))

    local input=""
    local cmd
    for cmd in "$@"; do
        input+="$cmd"$'\n'
    done

    run_mini "$input"

    local ok=1 reasons=()
    [[ "$MINI_STDOUT" != "$exp_out" ]] && {
        ok=0
        reasons+=("stdout  got=[$MINI_STDOUT]  want=[$exp_out]")
    }
    [[ "$MINI_EXIT" != "$exp_exit" ]] && {
        ok=0
        reasons+=("exit  got=$MINI_EXIT  want=$exp_exit")
    }

    print_result "$ok" 0 "$label" "${reasons[@]}"
}

run_file_test() {
    local label="$1" input="$2" filepath="$3" expected="$4"
    TOTAL=$((TOTAL + 1))
    rm -f "$filepath"

    run_mini "$input"
    local got=""
    [[ -f "$filepath" ]] && got=$(cat "$filepath")

    local ok=1 reasons=()
    [[ "$got" != "$expected" ]] && {
        ok=0
        reasons+=("file  got=[$got]  want=[$expected]")
    }

    print_result "$ok" 0 "$label" "${reasons[@]}"
}

setup

[[ ! -x "$MINI" ]] && {
    echo -e "${RED}Error: '$MINI' not found or not executable.${RESET}"
    teardown
    exit 1
}

echo -e "\n${BOLD}${CYAN}══════════════════════════════════════════${RESET}"
echo -e "${BOLD}${CYAN}   Minishell Mandatory Tester${RESET}"
echo -e "${BOLD}${CYAN}══════════════════════════════════════════${RESET}"

echo -e "\n${BOLD}──────── echo ────────${RESET}"
run_test 'echo hello world'          'echo hello world'
run_test 'echo "hello world"'        'echo "hello world"'
run_test "echo 'hello world'"        "echo 'hello world'"
run_test "echo hello'world'"         "echo hello'world'"
run_test 'echo hello""world'         'echo hello""world'
run_test "echo ''"                   "echo ''"
run_test 'echo ""'                   'echo ""'
run_test 'echo (no args)'            'echo'
run_test 'echo -n hello'             'echo -n hello'
run_test 'echo -n (no output)'       'echo -n'
run_test 'echo -nnn hello'           'echo -nnn hello'
run_test 'echo -n -n hello'          'echo -n -n hello'
run_test 'echo multiple   spaces'    'echo multiple   spaces'

echo -e "\n${BOLD}──────── echo: quotes & specials ────────${RESET}"
run_test "echo \"aspas ->'\""        "echo \"aspas ->'\""
run_test "echo '> >> < | # \$  <<'"  "echo '> >> < | # \$  <<'"
run_test 'echo "$"'                  'echo "$"'
run_test "echo '\$'"                 "echo '\$'"
run_test 'echo $'                    'echo $'

echo -e "\n${BOLD}──────── variable expansion ────────${RESET}"
run_test 'echo $?'                   'echo $?'
run_test 'echo $?HELLO'              'echo $?HELLO'
run_test 'echo "$PWD"'               'echo "$PWD"'
run_test "echo '\$PWD'"              "echo '\$PWD'"
run_test 'echo "$HOME"'              'echo "$HOME"'
run_test 'echo $UNDEFINED_VAR'       'echo $UNDEFINED_VAR'
run_test 'echo "$UNDEFINED_VAR"'     'echo "$UNDEFINED_VAR"'
run_seq  'export V=hello then echo $V' 'hello' 0 'export V=hello' 'echo $V'
run_seq  'export V= then echo $V'      ''      0 'export V='      'echo $V'

echo -e "\n${BOLD}──────── pwd ────────${RESET}"
run_test 'pwd'                       'pwd'
run_test 'pwd extra_arg'             'pwd extra_arg'

echo -e "\n${BOLD}──────── export ────────${RESET}"
run_test 'export HELLO=123'          'export HELLO=123'
run_seq  'export then echo'          '123' 0 'export HELLO=123' 'echo $HELLO'
WARN_STDERR=1; run_test 'export A- (invalid)'      'export A-'
run_test 'export HELLO="123 A-"'     'export HELLO="123 A-"'
run_test 'export hello world'        'export hello world'
WARN_STDERR=1; run_test 'export HELLO-=123'        'export HELLO-=123'
WARN_STDERR=1; run_test 'export ='                 'export ='
WARN_STDERR=1; run_test 'export 123'               'export 123'
run_test 'export HELLO=123 A'        'export HELLO=123 A'
SKIP_STDERR=1; WARN_EXIT=1; run_test 'export no args (format differs)' 'export'

echo -e "\n${BOLD}──────── unset ────────${RESET}"
run_test 'unset (no args)'           'unset'
run_seq  'unset HELLO'               ''  0 'export HELLO=1' 'unset HELLO' 'echo $HELLO'
run_seq  'unset HELLO1 HELLO2'       ' ' 0 'export HELLO1=1' 'export HELLO2=2' 'unset HELLO1 HELLO2' 'echo $HELLO1 $HELLO2'
run_seq  'unset HOME'                ''  0 'unset HOME' 'echo $HOME'
run_seq  'unset PATH'                ''  0 'unset PATH' 'echo $PATH'

echo -e "\n${BOLD}──────── cd ────────${RESET}"
run_seq  'cd $PWD succeeds'          '0'      0 'cd $PWD'    'echo $?'
run_seq  'cd nonexistent fails'      '1'      0 'cd 123123'  'echo $?'
run_seq  'cd no args → $HOME'        "$HOME"  0 'cd'         'pwd'
run_seq  'cd ..'                     '0'      0 'cd ..'      'echo $?'
run_seq  'cd - returns to prev dir'  "$(pwd)" 0 'cd /tmp' 'cd -' 'pwd'
run_seq  'cd updates $PWD'           '/tmp'   0 'cd /tmp' 'echo $PWD'
run_seq  'cd updates $OLDPWD'        '/tmp'   0 'cd /tmp' 'cd /' 'echo $OLDPWD'

echo -e "\n${BOLD}──────── exit ────────${RESET}"
run_test 'exit 0'                    'exit 0'
run_test 'exit 1'                    'exit 1'
run_test 'exit 42'                   'exit 42'
run_test 'exit 123'                  'exit 123'
run_test 'exit 298 (wraps)'          'exit 298'
run_test 'exit +100'                 'exit +100'
run_test 'exit -100'                 'exit -100'
WARN_STDERR=1; run_test 'exit hello'      'exit hello'
WARN_STDERR=1; run_test 'exit 42 world'   'exit 42 world'
run_seq  'exit no args uses last $?' '' 1 'false' 'exit'

echo -e "\n${BOLD}──────── pipes ────────${RESET}"
run_test 'echo hi | cat'             'echo hi | cat'
run_test 'echo hi | cat | cat'       'echo hi | cat | cat'
run_test 'echo hi | grep hi'         'echo hi | grep hi'
run_test 'echo hi | grep bye'        'echo hi | grep bye'
run_test 'cat infile | grep hello'   "cat $TEST_FILES/infile | grep hello"
run_test 'echo hi | wc -c'           'echo hi | wc -c'
run_test 'false | echo hi'           'false | echo hi'
run_seq  'pipe exit = last cmd false' '1' 0 'true | false' 'echo $?'
run_seq  'pipe exit = last cmd true'  '0' 0 'false | true' 'echo $?'

echo -e "\n${BOLD}──────── pipe syntax errors ────────${RESET}"
WARN_STDERR=1; run_test 'pipe at start'   '| echo hi'
WARN_STDERR=1; run_test 'pipe at end'     'echo hi |'
WARN_STDERR=1; run_test 'double pipe'     'echo hi | | cat'

echo -e "\n${BOLD}──────── redirections: input ────────${RESET}"
run_test 'cat < infile'              "cat <$TEST_FILES/infile"
run_test 'grep hello < infile'       "grep hello <$TEST_FILES/infile"
run_test 'cat < file with spaces'    "cat <\"$TEST_FILES/file name with spaces\""
run_test 'cat < missing'             "cat <$TEST_FILES/missing_xyz"
run_test 'double < last wins'        "cat <$TEST_FILES/infile_big <$TEST_FILES/infile"
run_test '< before cmd'              "< $TEST_FILES/infile cat"
run_test 'echo ignores stdin redir'  "echo hi <$TEST_FILES/infile bye"
run_test 'fail redir stops exec'     "echo <$TEST_FILES/infile <$TEST_FILES/missing_xyz <$TEST_FILES/infile"

echo -e "\n${BOLD}──────── redirections: output ────────${RESET}"
run_file_test 'echo hi > outfile' \
    "echo hi >$OUTFILES/o1" "$OUTFILES/o1" 'hi'
run_file_test 'echo hi > outfile bye' \
    "echo hi >$OUTFILES/o1 bye" "$OUTFILES/o1" 'hi bye'
run_file_test '> truncates existing' \
    "$(printf 'echo old >%s\necho new >%s' "$OUTFILES/o1" "$OUTFILES/o1")" \
    "$OUTFILES/o1" 'new'
run_file_test 'double > last file written' \
    "echo hi >$OUTFILES/o1 >$OUTFILES/o2" "$OUTFILES/o2" 'hi'
run_test '> invalid_permission'      "ls >$TEST_FILES/invalid_permission"

echo -e "\n${BOLD}──────── redirections: append ────────${RESET}"
run_file_test '>> creates file' \
    "echo hi >>$OUTFILES/o1" "$OUTFILES/o1" 'hi'
run_file_test '>> appends not truncates' \
    "$(printf 'echo first >>%s\necho second >>%s' "$OUTFILES/o1" "$OUTFILES/o1")" \
    "$OUTFILES/o1" "$(printf 'first\nsecond')"
run_test '>> invalid_permission'     "echo hi >>$TEST_FILES/invalid_permission"

echo -e "\n${BOLD}──────── redirections: mixed ────────${RESET}"
run_file_test 'cat < infile > outfile' \
    "cat <$TEST_FILES/infile >$OUTFILES/o1" "$OUTFILES/o1" "$(cat "$TEST_FILES/infile")"
run_test '< missing > outfile'       "cat <$TEST_FILES/missing_xyz >$OUTFILES/o1"
run_test '> outfile < missing'       "cat >$OUTFILES/o1 <$TEST_FILES/missing_xyz"

echo -e "\n${BOLD}──────── heredoc ────────${RESET}"
run_test 'basic heredoc' \
    "$(printf 'cat << EOF\nhello\nworld\nEOF')"
run_test "heredoc single-quoted delim (no expand)" \
    "$(printf "cat << 'EOF'\nhello \$HOME\nEOF")"
run_test 'heredoc unquoted delim (expands vars)' \
    "$(printf 'cat << EOF\nhello $HOME\nEOF')"
run_test 'heredoc multiline' \
    "$(printf 'cat << EOF\nline1\nline2\nline3\nEOF')"
run_test 'heredoc empty body' \
    "$(printf 'cat << EOF\nEOF')"
run_test 'heredoc double-quoted delim (no expand)' \
    "$(printf 'cat << "EOF"\nhello $HOME\nEOF')"
run_test 'heredoc piped to cat' \
    "$(printf 'cat << EOF | cat\nhello\nEOF')"

echo -e "\n${BOLD}──────── pipes + redirects combined ────────${RESET}"
run_test 'cat < infile | grep hello'  "cat <$TEST_FILES/infile | grep hello"
run_file_test 'cat < infile | cat > outfile' \
    "cat <$TEST_FILES/infile | cat >$OUTFILES/o1" "$OUTFILES/o1" "$(cat "$TEST_FILES/infile")"
run_test 'cat < missing | echo hi'   "cat <$TEST_FILES/missing_xyz | echo hi"
# Bash exits 1 for the whole script here, but the interactive shell itself should not die.
# Check semantic behavior via $? instead of process exit code.
run_seq  'cat < missing | cat then echo $?' '1' 0 "cat <$TEST_FILES/missing_xyz | cat" 'echo $?'

echo -e "\n${BOLD}──────── syntax errors ────────${RESET}"
WARN_STDERR=1; run_test 'redirect no filename <'  'echo hi <'
WARN_STDERR=1; run_test 'redirect no filename >'  'echo hi >'
WARN_STDERR=1; run_test 'heredoc no delimiter'    'cat <<'

echo -e "\n${BOLD}──────── external commands ────────${RESET}"
WARN_STDERR=1; run_test 'missing.out (127)'        'missing.out'
WARN_STDERR=1; run_test './missing.out (127)'      './missing.out'
WARN_STDERR=1; run_test '/nonexistent_xyz (127)'   '/nonexistent_xyz'
WARN_STDERR=1; run_test '"aaa" (127)'              '"aaa"'
WARN_STDERR=1; run_test 'invalid_permission (126)' "$TEST_FILES/invalid_permission"

echo -e "\n${BOLD}──────── empty / whitespace ────────${RESET}"
run_seq 'empty input' '' 0 ''
run_seq 'spaces only' '' 0 '   '

echo -e "\n${BOLD}──────── exit status \$? ────────${RESET}"
run_seq '$? after true'            '0'   0 'true' 'echo $?'
run_seq '$? after false'           '1'   0 'false' 'echo $?'
run_seq '$? in double quotes'      '1'   0 'false' 'echo "$?"'
run_seq '$? after missing cmd'     '127' 0 'missing_cmd_xyz' 'echo $?'
run_seq '$? after failed redir'    '1'   0 "cat <$TEST_FILES/missing_xyz" 'echo $?'
run_seq '$? pipe exit = last cmd'  '1'   0 'true | false' 'echo $?'

echo -e "\n${BOLD}──────── empty variable expansion ────────${RESET}"
run_seq '$EMPTY alone → exit 0'     ''   0 'export EMPTY=' '$EMPTY'
run_seq '$EMPTY echo hi → echo hi'  'hi' 0 'export EMPTY=' '$EMPTY echo hi'
run_seq 'unset var alone'           ''   0 '$UNDEFINED_VAR_XYZ_123'
run_seq 'unset var then cmd'        'hi' 0 '$UNDEFINED_VAR_XYZ_123 echo hi'

teardown

echo ""
echo -e "${BOLD}${CYAN}══════════════════════════════════════════${RESET}"
echo -e "${BOLD}  Results: ${GREEN}$PASS passed${RESET}  ${RED}$FAIL failed${RESET}  ${YELLOW}$WARN warnings${RESET}  $TOTAL total"
echo -e "${BOLD}${CYAN}══════════════════════════════════════════${RESET}"

if [[ $FAIL -eq 0 ]]; then
    echo -e "\n${GREEN}${BOLD}All tests passed.${RESET}\n"
else
    echo -e "\n${RED}${BOLD}$FAIL test(s) failed.${RESET}\n"
fi

exit $FAIL
