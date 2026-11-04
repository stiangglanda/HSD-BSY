/*CommandInterpreter.c*/
#include <string.h>
#include "CommandInterpreter.h"

//module internal members
static TCmdHndTable const* gcmds = 0;		//table of available commands
static int (*gpPutChar)(int);		         //function pointer for a put-char-function
static TParameters gParams;			      //struct that holds all entered params to last cmd
static char gInput[MAX_INPUTLEN];	      //modul internal puffer
static int gInputLen = 0;			         //length of input string

//error messages
static char* cErrPrint = "Error in Print(...) -> null pointer in parameter\n";


//////////////////////////////
// interface module functions
//////////////////////////////

void Init(TCmdHndTable const cmds[], int (*PutCh)(int))
{
   //to do
}

//pre declaration
static void evaluateCmd();
static void checkInput();

int Process(char const ch)
{
   if (ch == cESC)
   {
      return cESC;
   }

   //to do
   checkInput();

   return ch;
}


TParameters GetParams(){
	return gParams;
}


void Print(char const* str)
{
   //check parameter
	if (str == 0)
	{
      Print(cErrPrint);
      return;
	}

	int i = 0;
	if (gpPutChar!=0){		//put char function pointer given?
		while(str[i]!='\0'){	//iterate given string
			gpPutChar(str[i++]);
		}
	}
}

/////////////////////////////
// internal module functions
/////////////////////////////

//evaluates a completed command
static void evaluateCmd()
{
   //to do
}

//checks the input for correctness
static void checkInput()
{
   //to do
}




