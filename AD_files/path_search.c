#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path_search.h"

void search_for_command (char* command)
{
	char * mainPATH = getenv("PATH");

	char * copyPATH;
	strcpy(copyPATH, mainPATH);

	printf("mainPATH: %s\n", copyPATH);

	char ** parsedPATH = NULL;
	char * tok = strtok (copyPATH, ":");
	int size = 0;

	//allocates space in array for each individual path
	while(tok)
	{
		parsedPATH = realloc(parsedPATH, (sizeof(char*) * ++size));

		if (parsedPATH == NULL)
			exit(-1);

		parsedPATH[size-1] = tok;		//sets char pointer at size-1 to the token
		tok = strtok(NULL, ":");
	}

	//null-termination of array
	parsedPATH = realloc (parsedPATH, sizeof(char*) * size+1);
	parsedPATH[size] = 0;

	//append command to a temp variable that is overwritten in each iteration to check then next path
	
	int i;
	for (i = 0; i < (size+1); i++)
  	{
		if(parsedPATH[i] != NULL)
		{
			int size_0 = strlen(parsedPATH[i]) + strlen(command) + 1;
			char temp[size_0];
			strcpy(temp, parsedPATH[i]);
			strcat(temp, "/");
			strcat(temp, command);
			printf("temp at %d: %s\n", i, temp);
			if(does_command_exist(temp))
			{
				printf("Running Command\n");		//insert command execution here
				//break;
			}
			else
			{
				printf("command not found\n");
				//commandNotFoundCounter++;
			}
		}
	}
	printf("i is: %d\nsize is %d\n",i,size);
	//if (commandNotFoundCounter == i)	
	free(parsedPATH);
}

void execute_command(char* cmdpath)
{
	/*pid_t pid = fork();
	if (pid == 0)
	*/
}

// checks for exists of a command
int does_command_exist(char* path)
{
	FILE * check = fopen(path, "r");
	if (check == NULL)
		return 0;
	fclose(check);
	return 1;
}







