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
	assert(fileName != NULL);

   size_t const cLen = 32;
   size_t const cMaxPers = 2;

   //enum{ cLen = 32 };
   //enum{ cMaxPers = 2 };

   //structure for one person
   typedef struct Person
   {
      char name[cLen];
      unsigned short age;
   } Person;

   Person persGrp[cMaxPers] = { };

   // herkoemliche Initialisierung
   //memset(persGrp, 0, sizeof(persGrp));

   //init person
   strcpy(persGrp[0].name, "Robert \"Eisenfuss\" Pecl");
   strcpy(persGrp[1].name, "Anton \"Rambo\" Pfeffer");
   persGrp[0].age = 55;
   persGrp[1].age = 52;

   //open file for writing
   FILE* file = fopen(fileName, "w");
   if(file == NULL)
   {
      perror(cErrOpen);
      return NOK;
   }

   //binary writing of whole block
   if(fwrite(persGrp, sizeof(Person), cMaxPers, file) != cMaxPers)
   {
      perror(cErrWrite);
      fclose(file);
      return NOK;
   }

   //closeing
   if(fclose(file) == EOF)
   {
      perror(cErrClose);
      return NOK;
   }

   return OK;
}


//output file character by character
static short OutputChars(char const* const fileName)
{
 	//to do
	assert(fileName != NULL);

   FILE* file = fopen(fileName, "r");
   if(file == NULL)
   {
      perror(cErrOpen);
      return NOK;
   }

   //read to end of file
   int ch = EOF;
   while((ch = fgetc(file)) != EOF) { // auch im Fehlerfall EOF
      printf("Zeichen: '%c' -> ASCII: %d\n", ch, ch);
   }

   //check reading error 
   if(ferror(file) != 0)
   {
      perror(cErrRead);
      fclose(file);
      return NOK;
   }
   //check end of file
   if(feof(file) != 0)
   {
      puts(cFileEnd);
   }

   if(fclose(file) == EOF)
   {
      perror(cErrClose);
      return NOK;
   }

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
