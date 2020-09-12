#include "parser.h"
#include "sys/types.h"

void search_for_command (char*, tokenlist*, pid_t*, char**);
void execute_command(char*, tokenlist*, int, pid_t*, char**);
int does_command_exist(char*);
