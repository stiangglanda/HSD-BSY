#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

//error code
static short const OK = 0;
static short const NOK = 1;

//error and info messages
static char const* const cErrOpen = "error in fopen";
static char const* const cErrClose = "error in fclose";
static char const* const cErrWrite = "error in fwrite";
static char const* const cErrRead = "error in fgetc";
static char const* const cErrUsage = "error in commandline -> \
./readWrite fileName";
static char const* const cFileEnd = "end of file reached";


//writing a binary block to file
static short WriteABlock(char const * const fileName)
{
	//to do
	//....


   return OK;
}


//output file character by character
static short OutputChars(char const* const fileName)
{
 	//to do
	//...
   
   return OK;
}


//main program
int main(int argc, char** argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "%s\n", cErrUsage);
      return EXIT_FAILURE;
   }

   //uses the fopen, fclose and fwrite functions
   if (WriteABlock(argv[1]) == NOK)
   {
      return EXIT_FAILURE;
   }
   
   //reads character by character from a file
   if (OutputChars(argv[1]) == NOK)
   {
      return EXIT_FAILURE;
   }  
   return EXIT_SUCCESS;
}
