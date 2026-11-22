#ifndef DIRECTORY_H
#define DIRECTORY_H

/*
changes to directory in parameter path
*/
void ChangeToDir (char const* const path);

/*
prints the content of current directory and all sub directories to stdout 
*/
void PrintDir();

/*
adds a new directory to current directory 
*/
void AddDir (char const* const name);

/*
adds a new file to current directory
*/
void AddFile (char const* const name);

/*
removes all directories, subdirectories and files in current directory 
*/
void RemoveAll();

/*
sets access rights for new directories and files
*/
void SetAccessRights (mode_t const mode);

#endif // DIRECTORY_H