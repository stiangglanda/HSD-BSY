#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandInterpreter.h"
#include "CommandTable.h"


int main(int const argc, char const* const* argv){

   char ch = '\0';

   //initialize the command interpreter
   Init(GetCmdHndTable(), putchar);

   //read characters from input
   do {
      ch = getchar(); //wait for key
   } while(Process(ch)!= cESC);	//ESC typed

	return EXIT_SUCCESS;
}
