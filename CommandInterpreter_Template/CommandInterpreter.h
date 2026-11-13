/*CommandInterpreter.h*/
#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H
#include "CommandTable.h"

//Constants
#define MAX_INPUTLEN 128
#define MAX_PARAMS 16
#define MAX_PARAMLEN 32

//code for program quit
static int const cESC = 0x1B;

//return type of GetParams...holding a 2D Array
typedef struct{
	char params[MAX_PARAMS+1][MAX_PARAMLEN+1];
} TParameters;

//initialize Function. 1st Parameter: CommandTable, 2nd: a put char function
void Init(TCmdHndTable const cmds[], int (*PutCh)(int));

//provides access to all entered parameters of current command
//1st parameter is command itself
TParameters GetParams();

//function that handles every single incoming char
int Process(char const ch);

//function which writes a string to the given put-char-function
void Print(char const* str);

#endif
