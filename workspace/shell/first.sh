#!/bin/bash

name="Max"
number=4711
echo $name $number

# einlesen
echo "username:"
read user

# eine Bedingung
if [ "$user" = "Max" ]; then
    echo "Hallo $user"
else
    echo "You are not Max"
fi

# eine Schleife
for ((i=1; i<=3; ++i)); do
    echo "number: $i"
done

count=1
while [ $count -le 3 ]
do
    echo "run $count"
    count=$(($count+1))
done

# Funktionen
func_foo()
{
    echo "param1: $0" #Programm -> argv[0]
    echo "I'm $1"
}

compare()
{
    if [ "$1" = "$user" ]; then
        echo "equal"
    else
        echo "$1 and $user are not equal"
    fi
}

manipASrtring()
{
    if [ -z "$1" ]; then
        echo "empty"
    else
        echo "$1 not empty"
        echo "${1:0:3}" # Ausgabe der ersten 3 Zeichen
    fi
}

# Aufruf der Funktionen
func_foo "Anna"
compare "$name"
compare
manipASrtring "$user"