#include "directory.h"

int main(int argc, char* argv[])
{
   AddDir("new_dir");
   ChangeToDir("new_dir");
   AddFile("new_file.txt");
   //change to directory given in command line
   if(argc > 1)
   {
      ChangeToDir(argv[1]);
   }

   //print content of current directory and all sub directories
   PrintDir();

   //add a new directory
   //AddDir("new_dir");

   //add a new file
   //AddFile("new_file.txt");

   //remove all
   //RemoveAll();

   return 0;
}