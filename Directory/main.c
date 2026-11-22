#include "directory.h"

int main(int argc, char* argv[])
{
   AddDir("new_dir");
   ChangeToDir("new_dir");
   AddFile("new_file.txt");
   PrintDir();

   return 0;
}