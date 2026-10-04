#!/bin/bash

# This was developed on native Ubuntu (no WSL2), so /mnt/d/Workspaces/Ubuntu_Workspace
# does not exist here. A local directory is used instead.

folderToBackup="$HOME/Workspace"
finalDestination="$HOME/dev/HSD-BSY/Backup"

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <backup_directory> <backup_name>" >&2 # >&2 prints to stderr
    exit 1
fi
if [ ! -d "$1" ]; then
    echo "Error: backup directory '$1' does not exist" >&2
    exit 1
fi
if [ ! -d "$finalDestination" ]; then
    echo "Error: target directory '$finalDestination' does not exist" >&2
    exit 1
fi

timestamp=$(date "+%Y%m%d_%H%M%S")
zipLocation="$1/$2_$timestamp.zip"

echo "Backing up $folderToBackup to $1"
if ! zip -r "$zipLocation" "$folderToBackup"; then
    echo "Error: zip failed" >&2
    exit 1
fi
echo "backup created: $zipLocation"
echo "backup saved at: $zipLocation"

if ! mv "$zipLocation" "$finalDestination/"; then
    echo "Error: move failed" >&2
    exit 1
fi
echo "backup moved to: $finalDestination/$(basename "$zipLocation")"