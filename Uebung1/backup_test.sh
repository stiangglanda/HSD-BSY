#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source_folder> <destination_folder>"
    exit 0
fi

folderToBackup="~/Workspace"
BackupDestination="~/Backup"

echo "Backing up $folderToBackup to $BackupDestination"

#zip -r backup.zip mein_ordner