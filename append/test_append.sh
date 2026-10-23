#!/bin/bash

check_status()
{
    local status=$1
    local expected=$2

    echo "Exit-Status: $status"

    if [ "$expected" -eq 0 ]; then
        if [ "$status" -eq 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
        fi
    else
        if [ "$status" -ne 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
        fi
    fi
}

# Loeschen der generierten Dateien
rm -f append file1.txt file2.txt non_existing.txt large.bin

# Kompilieren des Programms
gcc -o append append.c -Wall -Wextra

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

echo "Falsche Argumente (zu wenige)"
./append
check_status $? 1

./append file1.txt
check_status $? 1

echo
echo "Falsche Argumente (zu viele)"
./append a b c
check_status $? 1

echo
echo "Quelldatei existiert nicht"
./append non_existing.txt file2.txt
check_status $? 1

echo
echo "Zieldatei existiert nicht"
echo "Erste Zeile aus file1" > file1.txt
./append file1.txt file2.txt
check_status $? 0
echo "Inhalt von file2.txt:"
cat file2.txt

echo
echo "Zieldatei existiert bereits"
echo "Zweite Zeile wird angehaengt" > file1.txt
./append file1.txt file2.txt
check_status $? 0
echo "Inhalt von file2.txt nach Append (sollte 2 Zeilen haben):"
cat file2.txt

echo
echo "Grosse Datei mit truncate"
truncate -s 1M large.bin
./append large.bin file2.txt
check_status $? 0

echo
echo "Dateigroessen:"
ls -lh file2.txt large.bin

# Aufraeumen
rm -f file1.txt file2.txt large.bin