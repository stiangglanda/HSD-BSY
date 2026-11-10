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

//texts
static char const* cWelcome = "CmdHnd: Welcome!\n";
static char const* cPrompt = ">";


//////////////////////////////
// interface module functions
//////////////////////////////

void Init(TCmdHndTable const cmds[], int (*PutCh)(int))
{
   //remember command table and output function
   gcmds = cmds;
   gpPutChar = PutCh;

   //start with empty input and empty parameters
   gInputLen = 0;
   gInput[0] = '\0';
   memset(&gParams, 0, sizeof(gParams));

   //greeting and first prompt
   Print(cWelcome);
   Print(cPrompt);
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

   if (ch == '\n' || ch == '\r')
   {
      //line is complete -> check it and run the command
      checkInput();
   }
   else if (gInputLen < MAX_INPUTLEN - 1)	//keep one place for '\0'
   {
      //store char in the buffer
      gInput[gInputLen++] = ch;
      gInput[gInputLen] = '\0';

      //echo char over the output function
      char echo[2] = { ch, '\0' };
      Print(echo);
   }
   //else: buffer full -> char is ignored

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
   int n = 0;		//number of found words (command + parameters)
   int i = 0;		//position in input buffer
   int found = 0;	//1 if command exists in table

   //split the input into words separated by spaces
   //word 0 is the command, all following words are parameters
   memset(&gParams, 0, sizeof(gParams));
   while (gInput[i] != '\0' && n < MAX_PARAMS + 1)
   {
      while (gInput[i] == ' ')		//skip spaces
      {
         i++;
      }
      if (gInput[i] == '\0')
      {
         break;
      }

      int len = 0;
      while (gInput[i] != '\0' && gInput[i] != ' ')
      {
         if (len < MAX_PARAMLEN)	//too long words are cut off
         {
            gParams.params[n][len++] = gInput[i];
         }
         i++;
      }
      gParams.params[n][len] = '\0';
      n++;
   }

   //search the command in the table, the table ends with a null entry
   if (gcmds != 0)
   {
      for (int k = 0; gcmds[k].func != 0; k++)
      {
         if (strcmp(gcmds[k].command, gParams.params[0]) == 0)
         {
            found = 1;

            //call the command, 0 means error, everything else is OK
            if (gcmds[k].func() != 0)
            {
               Print("OK\n");
            }
            else
            {
               Print("ERROR\n");
            }
            break;
         }
      }
   }

   if (!found)
   {
      Print("\nERROR: unknown command\n");
   }
}

//checks the input for correctness
static void checkInput()
{
   //find the first char that is not a space
   int i = 0;
   while (gInput[i] == ' ')
   {
      i++;
   }

   if (gInput[i] == '\0')
   {
      //empty line -> only a new line, no command
      Print("\n");
   }
   else
   {
      evaluateCmd();
   }

   //clear the buffer for the next line and show the prompt
   gInputLen = 0;
   gInput[0] = '\0';
   Print(cPrompt);
}