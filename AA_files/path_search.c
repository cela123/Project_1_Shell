#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path_search.h"

void search_for_path_command(char* command)
{
	char * mainPATH = getenv("PATH");

	//parse path into an array of c-strings
	char ** parsedPATH = NULL;
	char * tok = strtok (mainPATH, ":");
	int spaces = 0;

	//loop until tok false, to the proper amount of space for each sub-path of $PATH
	while(tok)
	{
		parsedPATH = realloc(parsedPATH, sizeof(char*) * ++spaces);

		if (parsedPATH == NULL)		//exit if mem-allocation fails
			exit(-1);

		parsedPATH[spaces-1] = tok;
		tok = strtok(NULL, ":");
	}

	//terminate array with NULL character
	parsedPATH = realloc (parsedPATH, sizeof(char*) * spaces+1);
	parsedPATH[spaces] = 0;

	for (int i = 0; i < (spaces+1); i++)
  		printf ("parsedPATH[%d] = %s\n", i, parsedPATH[i]);
	
	printf("The passed in command is %s\n", command);
	
}


