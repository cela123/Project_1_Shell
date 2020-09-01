#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path_search.h"

void search_and_execute_command (char* command)
{
	char * mainPATH = getenv("PATH");

	char ** parsedPATH = NULL;
	char * tok = strtok (mainPATH, ":");
	int size = 0;

	//allocates space based on the size of each individual path, also determines size of array
	while(tok)
	{
		parsedPATH = realloc(parsedPATH, (sizeof(char*) * ++size));

		if (parsedPATH == NULL)
			exit(-1);

		parsedPATH[size-1] = tok;
		tok = strtok(NULL, ":");

		//printf("%s\n",parsedPATH[size-1]);
	}

	//null-termination of array
	parsedPATH = realloc (parsedPATH, sizeof(char*) * size+1);
	parsedPATH[size] = 0;

	for (int i = 0; i < (size+1); i++)
  	{
		if(parsedPATH[i] != NULL)
		{
			int size_0 = strlen(parsedPATH[i]) + strlen(command) + 1;
			char temp[size_0];
			strcpy(temp, parsedPATH[i]);
			strcat(temp, "/");
			strcat(temp, command);
			printf("temp at %d: %s\n", i, temp);
		}
		//printf ("parsedPATH[%d] = %s\n", i, parsedPATH[i]);
	}
	
	printf("size unaltered: %d; size alted: %d\n", size, size-1);
	printf("The passed in command is %s\n", command);

	int count = 0;
	/*while(parsedPATH[count] != NULL)
	{
		free(parsedPATH[count]);
		count++;
	}*/

	free(parsedPATH);
}




