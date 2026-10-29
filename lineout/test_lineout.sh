#!/bin/bash

# Function to check the exit status
check_status()
{
    local status=$1
    local expected=$2

    echo "Exit status: $status"

    if [ "$expected" -eq 0 ] && [ "$status" -eq 0 ]; then
        echo "Test -> OK"
    elif [ "$expected" -ne 0 ] && [ "$status" -ne 0 ]; then
        echo "Test -> OK"
    else
        echo "Test -> NOK"
    fi
}

# Delete the generated files
rm -rf lineout file.txt empty.txt long.txt dir

# Build the program
gcc -o lineout lineout.c -Wall -Wextra

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

# Create the test files
seq 1 60 | sed 's/^/line /' > file.txt
truncate -s 0 empty.txt

# Test the program
echo "no arguments -> ./lineout"
./lineout
check_status $? 1

echo
echo "too many arguments -> ./lineout 1 file.txt extra"
./lineout 1 file.txt extra
check_status $? 1

echo
echo "file does not exist -> ./lineout 3 missing.txt"
./lineout 3 missing.txt
check_status $? 1

echo
echo "whole file (60 lines expected):"
./lineout file.txt | wc -l
check_status ${PIPESTATUS[0]} 0

echo
echo "single line (line 3 expected):"
./lineout 3 file.txt
check_status $? 0

echo
echo "range 2-10 (lines 2 to 10 expected):"
./lineout 2-10 file.txt
check_status $? 0

echo
echo "mixed segments 3,4-9,12,14- (lines 3 to 9, 12 and 14 to 60 expected):"
./lineout 3,4-9,12,14- file.txt
check_status $? 0

echo
echo "open borders -20,50- (lines 1 to 20 and 50 to 60 expected):"
./lineout -20,50- file.txt
check_status $? 0

echo
echo "overlapping and unordered 3,18-,11-15,18-20 (ascending, each line once):"
./lineout 3,18-,11-15,18-20 file.txt | head -n 12
check_status ${PIPESTATUS[0]} 0

echo
echo "segment behind the end of the file (no output expected):"
./lineout 100-200 file.txt
check_status $? 0

echo
echo "exactly 10 segments (10 lines expected):"
./lineout 1,3,5,7,9,11,13,15,17,19 file.txt | wc -l
check_status ${PIPESTATUS[0]} 0

echo
echo "11 segments are too many -> ./lineout 1,2,3,4,5,6,7,8,9,10,11 file.txt"
./lineout 1,2,3,4,5,6,7,8,9,10,11 file.txt
check_status $? 1

echo
echo "invalid text -> ./lineout 4x file.txt"
./lineout 4x file.txt
check_status $? 1

echo
echo "only a dash -> ./lineout - file.txt"
./lineout - file.txt
check_status $? 1

echo
echo "zero is not allowed -> ./lineout 0 file.txt"
./lineout 0 file.txt
check_status $? 1

echo
echo "start behind end -> ./lineout 5-3 file.txt"
./lineout 5-3 file.txt
check_status $? 1

echo
echo "empty segments -> ./lineout , file.txt"
./lineout , file.txt
check_status $? 1

echo
echo "empty file (no output expected):"
./lineout empty.txt
check_status $? 0

echo
echo "long line with 1000 characters (1 line expected):"
head -c 1000 /dev/zero | tr '\0' 'a' > long.txt
echo >> long.txt
./lineout long.txt | wc -l
check_status ${PIPESTATUS[0]} 0