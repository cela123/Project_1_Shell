//#include "parser.h"
//Part 5) $PATH search
//when a command is not starting with / or built-in
//search for the command within $PATH
//if not within $PATH then display an error message command not found

void search_for_command (char*, tokenlist*, int hasIO);
void execute_command(char*, tokenlist*, int hasIO);
int does_command_exist(char*);