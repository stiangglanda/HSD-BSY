#include "Print.h" // header not included
#include <stdio.h> // needed for printf
#include <string.h> // needed for strlen, strcpy, strcat
#include <stdlib.h> // needed for malloc, free

static char const* mErrorText[] = {"OK","NOK"};


void PrintResult(char const * const text, unsigned const errorCode )
{
   if (errorCode > 1) // check if errorCode is valid
   {
      printf("Error: Invalid error code %d\n",errorCode);
      return;
   }

   char* out = (char*)malloc(strlen(text) + strlen(mErrorText[errorCode]) + 2); 
   strcpy(out,text);
   strcat(out, " ");
   strcat(out,mErrorText[errorCode]);
   out[strlen(out)] = '\0';  
   printf("%s\n\n",out);
   free(out);
}


void PrintHeader(char const header[])
{
   unsigned headLen = 0;

   printf("\n%s\n",header);
   headLen = strlen(header);
   while (headLen > 0)
   {
      putc('=',stdout);
      headLen--;
   }
   printf("\n");
}


void PrintStrArr(char const * const * const arr, unsigned const len)
{
   size_t i=0;
   for (;i<len;i++)
   {
      printf("%s\n",arr[i]);
   }
}


void PrintIntArr(int* arr, int len)
{
	for (int i=0; i<len; ++i)
	{
		printf("%d\n",arr[i]);
	}
	return;
}
