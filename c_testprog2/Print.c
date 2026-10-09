#include "Print.h" // header not included
#include <stdio.h> // needed for printf
#include <string.h> // needed for strlen, strcpy, strcat
#include <stdlib.h> // needed for malloc, free

#define EXTRA_CHARS 2 // separator blank + terminating '\0'
#define UNDERLINE_CHAR '=' // underline character of PrintHeader

static char const * const mErrorText[] = {"OK","NOK"}; // module-internal and const
#define ERROR_COUNT (sizeof(mErrorText) / sizeof(mErrorText[0])) // replaces magic number 1


void PrintResult(char const * const text, unsigned const errorCode )
{
   if (text == NULL) // added pointer check
   {
      fprintf(stderr, "Error: text is NULL\n");
      return;
   }

   if (errorCode >= ERROR_COUNT) // check if errorCode is valid
   {
      fprintf(stderr, "Error: Invalid error code %u\n", errorCode); // stderr, %u for unsigned
      return;
   }

   char* out = (char*)malloc(strlen(text) + strlen(mErrorText[errorCode]) + EXTRA_CHARS);
   if (out == NULL) // added malloc check
   {
      fprintf(stderr, "Error: out of memory\n");
      return;
   }

   strcpy(out,text);
   strcat(out, " ");
   strcat(out,mErrorText[errorCode]);
   // removed out[strlen(out)] = '\0' strcat terminates the string
   printf("%s\n\n",out);
   free(out);
}


void PrintHeader(char const header[])
{
   if (header == NULL) // added pointer check
   {
      fprintf(stderr, "Error: header is NULL\n");
      return;
   }

   size_t headLen = strlen(header); // size_t instead of unsigned

   printf("\n%s\n",header);
   while (headLen > 0)
   {
      putc(UNDERLINE_CHAR,stdout);
      headLen--;
   }
   printf("\n");
}


void PrintStrArr(char const * const * const arr, unsigned const len)
{
   if (arr == NULL) // added pointer check
   {
      fprintf(stderr, "Error: arr is NULL\n");
      return;
   }

   size_t i=0;
   for (;i<len;i++)
   {
      printf("%s\n",arr[i]);
   }
}


void PrintIntArr(int const * const arr, unsigned const len)
{
   if (arr == NULL) // added pointer check
   {
      fprintf(stderr, "Error: arr is NULL\n");
      return;
   }

   for (unsigned i=0; i<len; ++i)
   {
      printf("%d\n",arr[i]);
   }
   // removed return
}