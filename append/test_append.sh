#!/bin/bash
# Script to test the program ./append source dest

# Function to check the exit status
# Parameters:
#   $1 ... actual exit status
#   $2 ... expected result (0 = success, 1 = error)
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
rm -rf append source.txt dest.txt empty.txt big.txt dir

# Build the program
gcc -o append append.c -Wall -Wextra

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

# Test the program
echo "no arguments -> ./append"
./append
check_status $? 1

echo
echo "too many arguments -> ./append a b c"
./append a b c
check_status $? 1

echo
echo "source file does not exist -> ./append missing.txt dest.txt"
./append missing.txt dest.txt
check_status $? 1

echo
echo "dest.txt does not exist and is created (6 bytes expected):"
echo "hello" > source.txt
./append source.txt dest.txt
check_status $? 0

echo
echo "append again (dest.txt must have 12 bytes):"
./append source.txt dest.txt
check_status $? 0
wc -c dest.txt

echo
echo "empty source file (0 bytes expected):"
truncate -s 0 empty.txt
./append empty.txt dest.txt
check_status $? 0
wc -c dest.txt

echo
echo "buffer limit, 512 and 513 bytes:"
truncate -s 512 big.txt
rm -f dest.txt
./append big.txt dest.txt
check_status $? 0
truncate -s 513 big.txt
rm -f dest.txt
./append big.txt dest.txt
check_status $? 0

echo
echo "large file, 1 MB:"
truncate -s 1M big.txt
rm -f dest.txt
./append big.txt dest.txt
check_status $? 0
wc -c dest.txt

echo
echo "same file -> ./append source.txt ./source.txt"
./append source.txt ./source.txt
check_status $? 1

echo
echo "destination is a directory -> ./append source.txt dir"
mkdir dir
./append source.txt dir
check_status $? 1