#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path_search.h"

void search_for_path_command(char* command)
{
	char * mainPATH = getenv("PATH");

	char ** parsedPATH = NULL;
	char * tok = strtok (mainPATH, ":");
	int size = 0;

	//allocates space based on the size of each individual path, also determines size of array
	while(tok)
	{
		parsedPATH = realloc(parsedPATH, sizeof(char*) * ++size);

		if (parsedPATH == NULL)
			exit(-1);

		parsedPATH[size-1] = tok;	
		tok = strtok(NULL, ":");
	}

	//null-termination of array
	parsedPATH = realloc (parsedPATH, sizeof(char*) * size+1);
	parsedPATH[size] = 0;

	//print for checking
	for (int i = 0; i < (size+1); i++)
  		printf ("parsedPATH[%d] = %s\n", i, parsedPATH[i]);
	
	printf("size unaltered: %d; size alted: %d", size, size-1);
	printf("The passed in command is %s\n", command);
	//printf("The strlen(pathParse) is: %d\n", strlen(pathParse));
}



