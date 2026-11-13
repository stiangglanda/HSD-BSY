#!/bin/bash

cd "$(dirname "$0")"
ERRORS=0

# Sends input $2 to the program and checks whether line $3 occurs
# ESC is sent automatically at the end; timeout terminates the program after 5 seconds
check()
{
    local out
    out=$(printf '%b\033' "$2" | timeout 5 ./cmd)

    if echo "$out" | grep -Fxq -- "$3"; then
        echo "Test -> OK   $1"
    else
        echo "Test -> NOK  $1 (line missing: $3)"
        ERRORS=$((ERRORS + 1))
    fi
}

# Like check, but line $3 must NOT occur
check_not()
{
    local out
    out=$(printf '%b\033' "$2" | timeout 5 ./cmd)

    if echo "$out" | grep -Fxq -- "$3"; then
        echo "Test -> NOK  $1 (line must not exist: $3)"
        ERRORS=$((ERRORS + 1))
    else
        echo "Test -> OK   $1"
    fi
}

# Remove and build the program
rm -f cmd
gcc -o cmd main.c CommandInterpreter.c CommandTable.c -Wall

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

# Start and end
check "welcome text" "" "CmdHnd: Welcome!"
check_not "nothing after ESC" "help\033int 5\n" "Parameter: 5"

# help
check "help" "help\n" "addint [value1] [value2] adds two integer values"
check "question mark" "?\n" "available commands:"

# int
check "int" "int 123\n" "Parameter: 123"
check "int negative" "int -42\n" "Parameter: -42"
check "int max" "int 2147483647\n" "Parameter: 2147483647"
check "int too big" "int 2147483648\n" "ERROR"
check "int text" "int abc\n" "ERROR"
check "int 3.14" "int 3.14\n" "ERROR"
check "int no parameter" "int\n" "ERROR"

# float
check "float" "float 3.14\n" "Parameter: 3.14"
check "float integer value" "float 5\n" "Parameter: 5"
check "float text" "float Donald\n" "ERROR"
check "float no parameter" "float\n" "ERROR"

# string
check "string" "string Donald\n" "Parameter: Donald"
check "string first word" "string Donald Duck\n" "Parameter: Donald"
check "string no parameter" "string\n" "ERROR"

# addint
check "addint" "addint 3 4\n" "Result: 3 + 4 = 7"
check "addint negative" "addint -3 4\n" "Result: -3 + 4 = 1"
check "addint big sum" "addint 2147483647 1\n" "Result: 2147483647 + 1 = 2147483648"
check "addint 1 parameter" "addint 3\n" "ERROR"
check "addint text" "addint 3 abc\n" "ERROR"

# Invalid commands and whitespace
check "unknown command" "foo\n" "ERROR: unknown command"
check "upper case" "INT 5\n" "ERROR: unknown command"
check_not "empty line" "\n" "ERROR: unknown command"
check "spaces around" "  int    5   \n" "Parameter: 5"
check "CR as line end" "int 7\r" "Parameter: 7"
check "after an error" "foo\nint 9\n" "Parameter: 9"

# Long input; the buffer is 127 characters long
BIG=$(printf '%0200d' 0 | tr 0 a)
check "too long line" "$BIG\n" "ERROR: unknown command"
check "more than 16 params" "int 5 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18\n" "Parameter: 5"

echo
if [ $ERRORS -eq 0 ]; then
    echo "ALL TESTS OK"
else
    echo "$ERRORS TESTS FAILED"
    exit 1
fi