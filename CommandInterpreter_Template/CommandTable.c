#include <stdio.h>
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
	{CmdHelp,       "help",  "show help text"},
	{CmdHelp,       "?",     "show help text"},
	{CmdParamString,"string","[value] first parameter as string"},
	{CmdParamInt,   "int" ,  "[value] first parameter as integer"},
	{CmdParamFloat, "float", "[value] first parameter as float"},
	{CmdAddInt, "addint", "[value1] + [value1] adds two integer values"},
	{0,0,0}  //end criteria
};


//returns address of command table
TCmdHndTable const* GetCmdHndTable()
{
	return cmds;
}


//////////////////////////////
// internal command functions
//////////////////////////////

//prints help text of all commandos
static int CmdHelp(){
	int i=0;
	Print("\n(press <ESC> to exit)\n");
	Print("available commands:\n");

	// to do

	return OK;
}


//interprets 1st parameter as integer and prints it
static int CmdParamInt(){
	int ret = NOK;	//return value

	// to do

	return ret;
}


//interprets 1st parameter as float and prints it
static int CmdParamFloat(){
	int ret = NOK;  //return value

	// to do

	return ret;
}


//interprets 1st parameter as string and prints it
static int CmdParamString()
{
	int ret = NOK;	//return value

	// to do

	return ret;
}


//adds two integer parameter and prints the result
static int CmdAddInt()
{
	int ret = NOK;  //return value

   // to do

	return ret;
}
