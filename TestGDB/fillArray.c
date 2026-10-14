#include <stdio.h>
#define NUM 10

void fillArray(int* arr)
{
	int i;
	for (i=0; i<NUM; i++)
	{
		arr[i] = i;
	}
}


int main(int argc, char** argv)
{
	int intArr[10];
	int i;
	
	for (i=0; i<argc; i++)
	{
		printf("Argument%d: %s\n",i,argv[i]);	
	}
	
	fillArray(intArr);
	for (i=0; i<NUM; i++)
	{
		printf("value %d: %d\n",i,intArr[i]);
	}
	return 0;
}
