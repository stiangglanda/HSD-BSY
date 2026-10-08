#include "Print.h" // header not included
#include <stdio.h> // needed for printf
#include <string.h> // needed for strlen, strcpy, strcat
#include <stdlib.h> // needed for malloc, free

#define MAX 100
#define BUFFER_LEN 1024


int TestFormatIO()
{
   int i = 0;
   char str[10] = "";  
   int j = 65;
   char *pCh = 0;
   double pi = 3.1415;

   // removed unused Arr
   // removed unused h

   PrintHeader("Test Format IO");
   // removed t
   i++;
   printf("%d ", i);
   printf("%p ", (void*)&i); // added (void*) to avoid warning

   printf("Bitte eingeben: ");
   fgets(str,BUFFER_LEN,stdin);
   printf("%s", str);
   printf("\n");
   
   fgets(str,BUFFER_LEN,stdin);
   
   int val = 0;
   scanf(str, "%4d", &val);
   printf("%d\n", val);

   pCh = (char *) &j;
   printf("%p\n", (void *)pCh); // added (void*) to avoid warning
   printf("%c \n", *pCh);

   printf("PI: %f \n", pi);	

   return 0; 
}


static void PrintLength(char buf[]);
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

   return 0;
}


static void PrintLength(char buf[]) {
   printf("Length of %s is %zu chars\n", buf, strlen(buf)); // changed %d to %zu for size_t
}


static void Shift(char v[]) {
   unsigned i = 0;

   for (i = 0; v[i]!=0; i++) {
      v[i]++;
   }
}


int TestDynMem()
{
   char *Buf = 0;

   PrintHeader("Test dynamic Memory");

   Buf = (char*)malloc(100);
   if (Buf != 0) {
      strcpy(Buf, "Hello World!");
      printf(" -> %s \n", Buf);

      free (Buf);
   }
   else
   {
      printf("Speicher konnte nicht reserviert werden!");
      return 1;
   }
   return 0;
}


int TestStruct()
{
   struct Person
   {
      char name[100];
      unsigned weight;
   }max; 

   struct Person moritz;
   
   PrintHeader("Test Structs");
   
   strcpy(moritz.name,"Moritz Mustermann");
   moritz.weight = 80;

   memset(&max,0,sizeof(struct Person));

   memcpy(max.name,"Max Mustermann",14);
   max.weight = moritz.weight;

   printf("Person: %s hat %d kg\n",max.name,max.weight);
   printf("Person: %s hat %d kg\n",moritz.name,moritz.weight);
   return 0;
}


int TestArray()
{
	int arr[10];
	
	memset(&arr,0,sizeof(arr));
	PrintHeader("initialized array with 0:");
	PrintIntArr(arr, 10); // needed len
	
	memset(&arr,1,sizeof(arr));
	PrintHeader("initialized array with 1:");
	PrintIntArr(arr, 10);
	
	return 0; // needed return value
}


static void PrintBackward(char str[])
{
	int i=0;
	int maxInx = strlen(str)-1;
	for (i=maxInx; i > -1; --i)
	{
		printf("%c",str[i]);
	}
	printf("\n");
}


static int comp (void const * str1, void const * str2)
{
   return strcmp(*(char**)str1,*(char**)str2);
}


typedef void (*TFunc) (char arr[]);


static void CallFuncPointer(TFunc func, char* arr[])
{
   unsigned i=0;
   for (;i<4;i++)
   {
      func(arr[i]);
   }
}


int TestFuncPtr()
{   

   const char* unsorted[] = {(char*)"Hello", (char*)"Martha", (char*)"Anton", (char*)"Berta"};
   
   void (*func) (char arr[]) = PrintLength;

   PrintHeader("Test Function Pointers");

   qsort(unsorted,4,sizeof(char*),comp);
   PrintStrArr(unsorted,4);

   CallFuncPointer(PrintBackward,unsorted);

   CallFuncPointer(func,unsorted);
   
   return 0;
}
