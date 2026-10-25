#!/bin/bash

check_status()
{
	#lokale variablen in check_status()
    local status=$1
    local expected=$2

    echo "Exit-Status: $status"

    if [ "$expected" -eq 0 ]; then
        # Erfolg wird erwartet
        if [ "$status" -eq 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
        fi
    else
        # Fehler wird erwartet
        if [ "$status" -ne 0 ]; then
            echo "Test -> OK"
        else
            echo "Test -> NOK"
        fi
    fi
}

# Loeschen der generierten Dateien
# rm -f -> force...ignoriere Warnungen beim Loeschen
rm -f write_file hugo maxi

# Erzeugen des Programms
gcc -o write_file write_file.c -Wall

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

# Teste das Programm
echo "create an error in the command line (no arguments) -> ./write_file"
./write_file
check_status $? 1

echo
echo "create an error in the command line (too many arguments) -> ./write_file hugo maxi"
./write_file hugo maxi
check_status $? 1

echo
echo "create file hugo and maxi:"
./write_file hugo
check_status $? 0

./write_file maxi
check_status $? 0

# Liste die Ergebnisse auf
echo
echo "list all generated files:"
ls -l hugo maxi

# Erzeuge die bestehenden Dateien erneut
echo
echo "create again existing files (-> error open file is expected):"
./write_file hugo
check_status $? 1

./write_file maxi
check_status $? 1

echo
echo "file content from hugo:"
cat hugo

echo
echo "file content from maxi:"
cat maxi