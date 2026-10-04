#!/bin/bash

folderToBackup="$HOME/dev/HSD-BSY/Workspace"
finalDestination="$HOME/dev/HSD-BSY/Backup"

if [ ! -d "$finalDestination" ]; then
    echo "$finalDestination does not exist" 
    exit 1
fi

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <destination_folder> <backup_name>"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "$1 does not exist" 
    exit 1
fi

timestamp=$(date "+%Y%m%d_%H%M%S")
ZipLocation="$1/$2_$timestamp.zip"

echo "Backing up $folderToBackup to $1"

zip -r "$ZipLocation" "$folderToBackup"

if [ $? -eq 0 ]; then
    echo "Backup successful: $ZipLocation"
else
    echo "Backup failed"
    exit 1
fi

mv "$ZipLocation" "$finalDestination"

if [ $? -eq 0 ]; then
    echo "move successful: $finalDestination"
else
    echo "move failed"
    exit 1
fi