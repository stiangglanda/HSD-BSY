//ist ein Feature-Test-Makro, das die Definition
//bestimmter Funktionen und Strukturen in den Header-Dateien
//ermöglicht, die sonst möglicherweise nicht verfügbar wären.
//z.B -> DT_REG, DT_DIR, d_type in struct dirent
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

//error messages
static char const* const cUsage       = "usage: ReadDirectory [path]\n";
static char const* const cErrOpenDir  = "error in opendir";
static char const* const cErrReadDir  = "error in readdir!";
static char const* const cErrCloseDir = "error in closedir!";

int main (int argc, char* argv[]) {
   int entries = 0;
   char* dirName = ".";  //current working direktory

   //check command line
   if (argc > 2)
   {
      fputs(cUsage,stderr);
      return EXIT_FAILURE;
   }

   if (argc == 2)
   {
      dirName = argv[1];
   }

   //to do

   //output current working directory
   char path[PATH_MAX] = {0};
   //to do

   return EXIT_SUCCESS;
}
