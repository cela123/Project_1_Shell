#include "parser.h"
#include "sys/types.h"

int hasPipe(char * command);
void piping(char*, tokenlist*, pid_t*, char**);
