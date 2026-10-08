#include "Test.h" // header not included
#include "Print.h" // header not included
#include <stdio.h> // needed for printf
#include <string.h> // needed for strlen, strcpy, strcat
#include <stdlib.h> // needed for malloc, free
#include <assert.h> // needed for assert in module-internal functions

#define MAX 100
#define BUFFER_LEN 10 // changed to 10 because of str
#define ARR_LEN 10 // length of the int test array
#define STR_COUNT 4 // number of strings in TestFuncPtr


int TestFormatIO()
{
   int i = 0;
   char str[BUFFER_LEN] = "";  // used the define
   int j = 65;
   char *pCh = NULL;
   double pi = 3.1415;

   // removed unused Arr
   // removed unused h

   PrintHeader("Test Format IO");
   // removed t
   i++;
   printf("%d ", i);
   printf("%p ", (void*)&i); // added (void*) to avoid warning

   printf("Bitte eingeben: ");
   if (fgets(str,BUFFER_LEN,stdin) == NULL) // added check of return value
   {
      fprintf(stderr, "Error: reading input failed\n");
      return TEST_NOK;
   }
   printf("%s", str);
   printf("\n");
   
   if (fgets(str,BUFFER_LEN,stdin) == NULL) // added check of return value
   {
      fprintf(stderr, "Error: reading input failed\n");
      return TEST_NOK;
   }
   
   int val = 0;
   if (sscanf(str, "%4d", &val) != 1) // scanf -> sscanf (parse from str), result checked
   {
      fprintf(stderr, "Error: no number entered\n");
      return TEST_NOK;
   }
   printf("%d\n", val);

   pCh = (char *) &j;
   printf("%p\n", (void *)pCh); // added (void*) to avoid warning
   printf("%c \n", *pCh);

   printf("PI: %f \n", pi);	

   return TEST_OK; // constant instead of 0
}


static void PrintLength(char const buf[]); // const added
static void Shift(char v[] );


int TestString()
{
   char buffer[MAX] = "";
   char text[] = "ABC";

   PrintHeader("Test Strings");

   strcpy(buffer, "Das ist ein C-String");
   PrintLength(buffer);

   printf("Buffer: %s \n", buffer);

   printf("Text vorher : %s \n", text);
   Shift(text);
   printf("Text nachher: %s \n", text);

   return TEST_OK;
}


static void PrintLength(char const buf[]) {
   assert(buf != NULL); // added assert
   printf("Length of %s is %zu chars\n", buf, strlen(buf)); // changed %d to %zu for size_t
}

static void Shift(char v[]) {
   assert(v != NULL); // added assert
   unsigned i = 0;

   for (i = 0; v[i]!=0; i++) {
      v[i]++;
   }
}


int TestDynMem()
{
   char *Buf = NULL;

   PrintHeader("Test dynamic Memory");

   Buf = (char*)malloc(MAX); // magic number replaced
   if (Buf != NULL) {
      strcpy(Buf, "Hello World!");
      printf(" -> %s \n", Buf);

      free (Buf);
      Buf = NULL; // no dangling pointer
   }
   else
   {
      fprintf(stderr, "Speicher konnte nicht reserviert werden!\n"); // stderr and newline added
      return TEST_NOK;
   }
   return TEST_OK;
}


int TestStruct()
{
   struct Person
   {
      char name[MAX];
      unsigned weight;
   };

   struct Person moritz;
   
   PrintHeader("Test Structs");
   
   strcpy(moritz.name,"Moritz Mustermann");
   moritz.weight = 80;

   struct Person max = {
      .name = "Max Mustermann",
      .weight = moritz.weight
   };

   printf("Person: %s hat %u kg\n",max.name,max.weight); // %u for unsigned
   printf("Person: %s hat %u kg\n",moritz.name,moritz.weight); // %u for unsigned
   return TEST_OK;
}


int TestArray()
{
	int arr[ARR_LEN];
	
	memset(arr,0,sizeof(arr)); // arr instead of &arr
	PrintHeader("initialized array with 0:");
	PrintIntArr(arr, ARR_LEN); // needed len
	
	for (unsigned i = 0; i < ARR_LEN; ++i) // replaced memsets
	{
		arr[i] = 1;
	}
	PrintHeader("initialized array with 1:");
	PrintIntArr(arr, ARR_LEN);
	
	return TEST_OK; // needed return value
}


static void PrintBackward(char const str[]) // const added
{
	assert(str != NULL);
	size_t i = strlen(str); // changed to size_t
	while (i > 0)
	{
		--i;
		printf("%c",str[i]);
	}
	printf("\n");
}


static int comp (void const * str1, void const * str2)
{
   assert(str1 != NULL && str2 != NULL); // added assert
   return strcmp(*(char const * const *)str1,*(char const * const *)str2); // added const to cast
}


typedef void (*TFunc) (char const arr[]); // added const for PrintLength and PrintBackward


static void CallFuncPointer(TFunc func, char const * const arr[], unsigned const len)
{
   assert(func != NULL && arr != NULL); // added assert
   unsigned i=0;
   for (;i<len;i++) // used len instead of magic number
   {
      func(arr[i]);
   }
}


int TestFuncPtr()
{   

   char const * unsorted[] = {"Hello", "Martha", "Anton", "Berta"}; // const casts removed
   
   TFunc func = PrintLength; // used typedef

   PrintHeader("Test Function Pointers");

   qsort(unsorted,STR_COUNT,sizeof(unsorted[0]),comp);
   PrintStrArr(unsorted,STR_COUNT); // cast not needed anymore

   CallFuncPointer(PrintBackward,unsorted,STR_COUNT);

   CallFuncPointer(func,unsorted,STR_COUNT);
   
   return TEST_OK;
}