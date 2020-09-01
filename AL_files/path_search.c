#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path_search.h"

void search_and_execute_command (char* command)
{
	char * mainPATH = getenv("PATH");

	char * copyPATH;
	strcpy(copyPATH, mainPATH);

	printf("mainPATH: %s\n", copyPATH);

	char ** parsedPATH = NULL;
	char * tok = strtok (copyPATH, ":");
	int size = 0;

	//allocates space based on the size of each individual path, also determines size of array
	while(tok)
	{
		parsedPATH = realloc(parsedPATH, (sizeof(char*) * ++size));

		if (parsedPATH == NULL)
			exit(-1);

		parsedPATH[size-1] = tok;
		tok = strtok(NULL, ":");
	}

	//null-termination of array
	parsedPATH = realloc (parsedPATH, sizeof(char*) * size+1);
	parsedPATH[size] = 0;

	//append command to a temp variable that is overwritten in each iteration to check then next path
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
			if(doesFileExist(temp))
			{
				printf("The command at %s exists\n", temp);
				break;
			}
			else
				printf("The command at %s doesn't exists\n", temp);
		}
	}
	
	free(parsedPATH);
}

int doesFileExist(char* path)
{
	FILE * check = fopen(path, "r");
	if (check == NULL)
		return 0;
	fclose(check);
	return 1;
}






