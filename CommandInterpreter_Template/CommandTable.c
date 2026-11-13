#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <memory.h>
#include "CommandTable.h"
#include "CommandInterpreter.h"

//return code for command functions
int const OK = 1;
int const NOK = 0;

//internal command functions
static int CmdHelp();
static int CmdParamInt();
static int CmdParamFloat();
static int CmdParamString();
static int CmdAddInt();

//CommandTable
static TCmdHndTable const cmds[] = {
	{CmdHelp,        "help",   "show help text"},
	{CmdHelp,        "?",      "show help text"},
	{CmdParamInt,    "int",    "[value] first parameter as integer"},
	{CmdParamFloat,  "float",  "[value] first parameter as float"},
	{CmdParamString, "string", "[value] first parameter as string"},
	{CmdAddInt,      "addint", "[value1] [value2] adds two integer values"},
	{0,0,0}  //end criteria
};


//returns address of command table
TCmdHndTable const* GetCmdHndTable()
{
	return cmds;
}


//////////////////////////////
// internal helper functions
//////////////////////////////

//converts a string to an int value
//returns 1 if the whole string is a valid int, otherwise 0
static int ToInt(char const* str, int* value)
{
	char* end = 0;
	long long result;

	if (str[0] == '\0')	//parameter is missing
	{
		return 0;
	}

	errno = 0;
	result = strtoll(str, &end, 10);

	//the whole string must be used and the value must fit into an int
	if (*end != '\0' || errno != 0 || result < INT_MIN || result > INT_MAX)
	{
		return 0;
	}

	*value = (int)result;
	return 1;
}

//converts a string to a float value
//returns 1 if the whole string is a valid number, otherwise 0
static int ToFloat(char const* str, double* value)
{
	char* end = 0;
	double result;

	if (str[0] == '\0')		//parameter is missing
	{
		return 0;
	}

	errno = 0;
	result = strtod(str, &end);

	//the whole string must be used
	if (*end != '\0' || errno != 0)
	{
		return 0;
	}

	*value = result;
	return 1;
}


//////////////////////////////
// internal command functions
//////////////////////////////

//prints help text of all commandos
static int CmdHelp(){
	int i=0;
	Print("\n(press <ESC> to exit)\n");
	Print("available commands:\n");

	//print every command with its description, table ends with a null entry
	for (i = 0; cmds[i].func != 0; i++)
	{
		Print(cmds[i].command);
		Print(" ");
		Print(cmds[i].description);
		Print("\n");
	}

	return OK;
}


//interprets 1st parameter as integer and prints it
static int CmdParamInt(){
	int ret = NOK;
	int value = 0;
	char text[64];
	TParameters p = GetParams(); //params[0] is the command itself

	Print("\n");
	if (ToInt(p.params[1], &value))
	{
		snprintf(text, sizeof(text), "Parameter: %d\n", value);
		Print(text);
		ret = OK;
	}

	return ret;
}


//interprets 1st parameter as float and prints it
static int CmdParamFloat(){
	int ret = NOK;
	double value = 0.0;
	char text[64];
	TParameters p = GetParams();

	Print("\n");
	if (ToFloat(p.params[1], &value))
	{
		snprintf(text, sizeof(text), "Parameter: %g\n", value);
		Print(text);
		ret = OK;
	}

	return ret;
}


//interprets 1st parameter as string and prints it
static int CmdParamString()
{
	int ret = NOK;
	TParameters p = GetParams();

	Print("\n");
	if (p.params[1][0] != '\0')		//parameter given?
	{
		Print("Parameter: ");
		Print(p.params[1]);
		Print("\n");
		ret = OK;
	}

	return ret;
}


//adds two integer parameter and prints the result
static int CmdAddInt()
{
	int ret = NOK;
	int a = 0;
	int b = 0;
	char text[96];
	TParameters p = GetParams();

	Print("\n");
	if (ToInt(p.params[1], &a) && ToInt(p.params[2], &b))
	{
		//use long long so that the sum can not overflow
		snprintf(text, sizeof(text), "Result: %d + %d = %lld\n", a, b, (long long)a + b);
		Print(text);
		ret = OK;
	}

	return ret;
}