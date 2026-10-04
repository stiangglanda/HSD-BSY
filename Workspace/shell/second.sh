#!/bin/bash

#anderes script einbinden
source first.sh

# exit-Status pruefen
if [ $? -eq 0 ]; then
    echo "run of $0 ok"
else
    echo "run of $0 nok"
fi