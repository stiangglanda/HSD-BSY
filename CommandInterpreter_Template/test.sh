#!/bin/bash

# Testscript fuer den Kommando-Interpreter (Uebung 6)
# Jeder Test schickt Eingaben an das Programm und prueft die Ausgabe.
# Am Ende der Eingabe wird automatisch ESC gesendet, sonst wuerde das Programm nie enden.

cd "$(dirname "$0")"

OUT=""          # Ausgabe des letzten Programmlaufs
STATUS=0        # Exit-Status des letzten Programmlaufs
ERRORS=0        # Anzahl der fehlgeschlagenen Pruefungen
TESTS=0         # Anzahl aller Pruefungen

check_status()
{
    #lokale variablen in check_status()
    local status=$1
    local expected=$2

    echo "Exit-Status: $status"
    TESTS=$((TESTS + 1))

    if [ "$expected" -eq 0 ]; then
        # Erfolg wird erwartet
        if [ "$status" -eq 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
            ERRORS=$((ERRORS + 1))
        fi
    else
        # Fehler wird erwartet
        if [ "$status" -ne 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
            ERRORS=$((ERRORS + 1))
        fi
    fi
}

# Startet das Programm mit der Eingabe $1 (printf-Format, z.B. "int 5\n")
# timeout 5 -> das Programm wird nach 5 Sekunden beendet, falls es haengt
run()
{
    echo
    echo "input: $1"
    OUT=$(printf '%b\033' "$1" | timeout 5 ./cmd)
    STATUS=$?
}

# Prueft ob eine Zeile genau $1 in der Ausgabe vorkommt
expect_line()
{
    TESTS=$((TESTS + 1))
    if echo "$OUT" | grep -Fxq -- "$1"; then
        echo "Test -> OK   (line found: $1)"
    else
        echo "Test -> NOK  (line missing: $1)"
        ERRORS=$((ERRORS + 1))
    fi
}

# Prueft ob eine Zeile genau $1 NICHT in der Ausgabe vorkommt
expect_no_line()
{
    TESTS=$((TESTS + 1))
    if echo "$OUT" | grep -Fxq -- "$1"; then
        echo "Test -> NOK  (line must not exist: $1)"
        ERRORS=$((ERRORS + 1))
    else
        echo "Test -> OK   (line not found: $1)"
    fi
}

# Erwartet einen erfolgreichen Befehl: Zeile $1 und OK
expect_ok()
{
    expect_line "$1"
    expect_line "OK"
    expect_no_line "ERROR"
}

# Erwartet einen fehlerhaften Befehl: ERROR und keine Ausgabezeile mit Parameter oder Result
expect_error()
{
    expect_line "ERROR"
    expect_no_line "OK"
    if echo "$OUT" | grep -Eq '^(Parameter|Result):'; then
        TESTS=$((TESTS + 1))
        echo "Test -> NOK  (no result line expected)"
        ERRORS=$((ERRORS + 1))
    fi
}

# Erwartet einen unbekannten Befehl
expect_unknown()
{
    expect_line "ERROR: unknown command"
    expect_no_line "OK"
}

# Loeschen der generierten Dateien
# rm -f -> force...ignoriere Warnungen beim Loeschen
rm -f cmd

# Erzeugen des Programms
gcc -o cmd main.c CommandInterpreter.c CommandTable.c -Wall

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

###########################################################
echo
echo "=== 1. Start und Ende ==="

run ""
expect_line "CmdHnd: Welcome!"
expect_line ">"
check_status $STATUS 0

run "int 5\n"
echo "echo of the typed command is expected:"
expect_line ">int 5"

run "help\033int 5\n"
echo "nothing after ESC must be processed:"
expect_no_line "Parameter: 5"
check_status $STATUS 0

###########################################################
echo
echo "=== 2. help und ? ==="

run "help\n"
expect_line "(press <ESC> to exit)"
expect_line "available commands:"
expect_line "help show help text"
expect_line "? show help text"
expect_line "int [value] first parameter as integer"
expect_line "float [value] first parameter as float"
expect_line "string [value] first parameter as string"
expect_line "addint [value1] [value2] adds two integer values"
expect_line "OK"

run "?\n"
expect_line "available commands:"
expect_line "OK"

run "help 1 2 3\n"
echo "extra parameters are ignored:"
expect_line "available commands:"
expect_line "OK"

###########################################################
echo
echo "=== 3. int ==="

run "int 123\n"
expect_ok "Parameter: 123"

run "int 0\n"
expect_ok "Parameter: 0"

run "int -42\n"
expect_ok "Parameter: -42"

run "int +5\n"
expect_ok "Parameter: 5"

run "int 007\n"
expect_ok "Parameter: 7"

run "int 2147483647\n"
echo "biggest int:"
expect_ok "Parameter: 2147483647"

run "int -2147483648\n"
echo "smallest int:"
expect_ok "Parameter: -2147483648"

run "int 2147483648\n"
echo "too big for int:"
expect_error

run "int -2147483649\n"
echo "too small for int:"
expect_error

run "int 99999999999999999999999999\n"
echo "way too big:"
expect_error

run "int\n"
echo "parameter is missing:"
expect_error

run "int \n"
echo "only a space after the command:"
expect_error

run "int abc\n"
expect_error

run "int 12abc\n"
expect_error

run "int 3.14\n"
expect_error

run "int -\n"
expect_error

run "int 0x10\n"
expect_error

run "int 5 6 7\n"
echo "extra parameters are ignored:"
expect_ok "Parameter: 5"

###########################################################
echo
echo "=== 4. float ==="

run "float 3.14\n"
expect_ok "Parameter: 3.14"

run "float -2.5\n"
expect_ok "Parameter: -2.5"

run "float 5\n"
echo "integer value is a valid float:"
expect_ok "Parameter: 5"

run "float 0\n"
expect_ok "Parameter: 0"

run "float .5\n"
expect_ok "Parameter: 0.5"

run "float 1e3\n"
expect_ok "Parameter: 1000"

run "float Donald\n"
expect_error

run "float 3.14abc\n"
expect_error

run "float 1.2.3\n"
expect_error

run "float\n"
echo "parameter is missing:"
expect_error

run "float 1e400\n"
echo "out of range:"
expect_error

run "float 1.5 2.5\n"
echo "extra parameters are ignored:"
expect_ok "Parameter: 1.5"

###########################################################
echo
echo "=== 5. string ==="

run "string Donald\n"
expect_ok "Parameter: Donald"

run "string 123\n"
echo "numbers are strings too:"
expect_ok "Parameter: 123"

run "string\n"
echo "parameter is missing:"
expect_error

run "string Donald Duck\n"
echo "only the first word is used:"
expect_ok "Parameter: Donald"

run "string a-b_c!\n"
expect_ok "Parameter: a-b_c!"

# 40 mal 'a' -> nach MAX_PARAMLEN (32) Zeichen wird abgeschnitten
LONG=$(printf '%040d' 0 | tr 0 a)
CUT=${LONG:0:32}
run "string $LONG\n"
echo "too long word is cut after 32 chars:"
expect_ok "Parameter: $CUT"

###########################################################
echo
echo "=== 6. addint ==="

run "addint 3 4\n"
expect_ok "Result: 3 + 4 = 7"

run "addint -3 4\n"
expect_ok "Result: -3 + 4 = 1"

run "addint -3 -4\n"
expect_ok "Result: -3 + -4 = -7"

run "addint 0 0\n"
expect_ok "Result: 0 + 0 = 0"

run "addint 2147483647 1\n"
echo "sum is bigger than int, must still be right:"
expect_ok "Result: 2147483647 + 1 = 2147483648"

run "addint -2147483648 -1\n"
expect_ok "Result: -2147483648 + -1 = -2147483649"

run "addint 3\n"
echo "second parameter is missing:"
expect_error

run "addint\n"
echo "both parameters are missing:"
expect_error

run "addint 3 abc\n"
expect_error

run "addint abc 4\n"
expect_error

run "addint 3 4.5\n"
expect_error

run "addint 2147483648 1\n"
echo "first value too big for int:"
expect_error

run "addint 3 4 5\n"
echo "extra parameters are ignored:"
expect_ok "Result: 3 + 4 = 7"

###########################################################
echo
echo "=== 7. ungueltige Kommandos ==="

run "foo\n"
expect_unknown

run "INT 5\n"
echo "commands are case sensitive:"
expect_unknown

run "in 5\n"
echo "part of a command:"
expect_unknown

run "inta 5\n"
echo "command with extra char:"
expect_unknown

run "5\n"
expect_unknown

run "int5\n"
echo "no space between command and parameter:"
expect_unknown

###########################################################
echo
echo "=== 8. Leerzeichen und Zeilenende ==="

run "\n"
echo "empty line, no command:"
expect_no_line "ERROR"
expect_no_line "ERROR: unknown command"
expect_no_line "OK"

run "     \n"
echo "only spaces:"
expect_no_line "ERROR"
expect_no_line "ERROR: unknown command"
expect_no_line "OK"

run "   int 5\n"
echo "spaces in front of the command:"
expect_ok "Parameter: 5"

run "int      5\n"
echo "many spaces between command and parameter:"
expect_ok "Parameter: 5"

run "int 5     \n"
echo "spaces at the end:"
expect_ok "Parameter: 5"

run "addint   3     4   \n"
expect_ok "Result: 3 + 4 = 7"

run "int 7\r"
echo "CR as end of line:"
expect_ok "Parameter: 7"

run "int 1\nint 2\nint 3\n"
echo "several commands one after the other:"
expect_line "Parameter: 1"
expect_line "Parameter: 2"
expect_line "Parameter: 3"

run "foo\nint 9\n"
echo "valid command after an invalid one:"
expect_line "ERROR: unknown command"
expect_line "Parameter: 9"

run "float Donald\nint 9\n"
echo "valid command after an error:"
expect_line "ERROR"
expect_line "Parameter: 9"

###########################################################
echo
echo "=== 9. Grenzen der Puffer ==="

# 200 Zeichen ohne Leerzeichen -> Puffer ist nur 127 Zeichen lang
BIG=$(printf '%0200d' 0 | tr 0 a)
run "$BIG\n"
echo "line longer than the input buffer, no crash:"
expect_unknown
check_status $STATUS 0

run "string $BIG\n"
echo "long parameter, buffer is full:"
expect_ok "Parameter: $CUT"
check_status $STATUS 0

run "int 5 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20\n"
echo "more parameters than MAX_PARAMS (16):"
expect_ok "Parameter: 5"
check_status $STATUS 0

run "int 5\nint 6\nint 7\nint 8\nint 9\nint 10\nint 11\nint 12\n"
echo "buffer is cleared after every line:"
expect_line "Parameter: 5"
expect_line "Parameter: 12"
expect_no_line "ERROR"

###########################################################
echo
echo "=== Ergebnis ==="
echo "tests: $TESTS, failed: $ERRORS"

if [ $ERRORS -eq 0 ]; then
    echo "ALL TESTS OK"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
