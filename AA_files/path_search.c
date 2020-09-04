#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parser.h"
#include "path_search.h"

void search_for_command (char* command, tokenlist* tokens)
{
	char * mainPATH = getenv("PATH");

	char * copyPATH = malloc(strlen(mainPATH));
	strcpy(copyPATH, mainPATH);
	
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
	for (i = 0; i < size; i++)
  	{
		if(parsedPATH[i] != NULL)
		{
			int size_0 = strlen(parsedPATH[i]) + strlen(command) + 1;
			//char temp_command[size_0];		//needs to be dynamically allocated
			char * temp_command = (char*)malloc(size_0);
			strcpy(temp_command, parsedPATH[i]);
			strcat(temp_command, "/");
			strcat(temp_command, command);
			//printf("temp_command at %d: %s\n", i, temp_command);
			if(does_command_exist(temp_command))
			{
				printf("Running Command\n");		//insert command execution here
				execute_command(temp_command, tokens);
				printf("After running command\n");
				
				break;		
			}
		}
	}

	if (size == i)
		printf("command not found\n");

	free(parsedPATH);
}

void execute_command(char* cmdpath, tokenlist* tokens)
{
	//free(tokens->items[0]);	//only if search_for_command is called first
	tokens->items[0] = cmdpath;

	for (int i = 0; i < tokens->size + 1; i++)
		printf("token %d: (%s)\n", i, tokens->items[i]);

	pid_t pid = fork();
	if (pid == 0)		//for some reason, this is getting called more then once occassionally
	{
		execv(tokens->items[0], tokens->items);		//replace [0] in tokens with the cmdpath
	}
	else
	{
		waitpid(pid, NULL, 0);
		printf("Child exited\n");
	} //need piping logic before and after the fork
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






