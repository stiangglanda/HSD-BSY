#ifndef COMMAND_TABLE
#define COMMAND_TABLE

//type of command table
typedef struct {
	int (*func)(void);
	char* command;
	char* description;
}TCmdHndTable;

//returns address of command table
TCmdHndTable const* GetCmdHndTable();


#endif //COMMAND_TABLE
