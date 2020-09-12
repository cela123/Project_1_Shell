#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "path_search.h"

void search_for_command (char* command, tokenlist* tokens, pid_t* bg_process, char** bg_commands)
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
			char * temp_command = (char*)malloc(size_0);
			strcpy(temp_command, parsedPATH[i]);
			strcat(temp_command, "/");
			strcat(temp_command, command);
			//printf("temp_command at %d: %s\n", i, temp_command);
			if(does_command_exist(temp_command))
			{
				//printf("Running Command\n");		//insert command execution here
				execute_command(temp_command, tokens, 1, bg_process, bg_commands);
				//printf("After running command\n");
				
				break;		
			}
		}
	}

	if (size == i)
		printf("command not found\n");

	free(parsedPATH);
}



void execute_command(char* cmdpath, tokenlist* tokens, int checkCallLocation, pid_t* bg_process, char** bg_commands)
{
	//printf("entering execute_command()\n");
	int isOutput = 0;
	int isBackground = -1;
	int isBackgroundFound = 0;
	int isInput = 0;
	int isBoth = 0;
	int isPipe = 0;
	int fd_in = 0; 
	int fd_out = -1;
	int store_out_index = -1;
	int store_in_index = -1;
	int lowest_index = 0;
	int i_count = 0;
	int k_count = 0;
	char * infile = NULL;
	char * outfile = NULL;
	tokenlist * temp_io = new_tokenlist();
	tokenlist * temp_bg = new_tokenlist();

	if(checkCallLocation == 1)
		free(tokens->items[0]);	//free memory at this space so we can replace

	tokens->items[0] = cmdpath;

	//<------------IO Processing Logic ---------------------->
	for(int i = 0; i < tokens->size-1; i++)
	{
		printf("token %d: (%s)\n", i, tokens->items[i]);
		if(strcmp(tokens->items[i], ">") == 0)
		{
			//printf("found output redir '>'\n"); 
			isOutput = 1;
			store_out_index = i;			//location of > if it exists
			outfile = tokens->items[i+1];	//outfile will be token after >

			if((fd_out = open(outfile, O_WRONLY | O_CREAT, 0644)) < 0)
			{
				perror("Output file could not be opened");
				return;
			}
		}
		if(strcmp(tokens->items[i], "<") == 0)
		{
			//printf("found input redir '<'\n"); 
			isInput = 1; 	
			store_in_index = i;
			infile = tokens->items[i+1];
			
			if((fd_in = open(infile, O_RDONLY, 0644)) < 0)
			{
				perror("Input file could not be opened");
				return;
			}
		}
	} 

	if(store_in_index == -1 && store_out_index > 0)
	{
		//printf("there is only output redirection\n\n"); 
		for (int k = 0; k < store_out_index; k++)
			add_token(temp_io, tokens->items[k]);
	}
	else if(store_in_index > 0 && store_out_index == -1)
	{
		//printf("there is only input redirection\n\n"); 
		for (int k = 0; k < store_in_index; k++)
			add_token(temp_io, tokens->items[k]);
	}
	else if(store_in_index > 0 && store_out_index > 0)
	{
		//printf("there is both input and output redirection\n");
		if(store_in_index > store_out_index)
			lowest_index = store_out_index;
		else
			lowest_index = store_in_index;
		
		for(int k = 0; k < lowest_index; k++)
			add_token(temp_io, tokens->items[k]);
	}
	//<------------IO Processing Logic end ------------------->

	//<------------Background processing logic ---------------> 
	//determine if there is both input and output or not for background processing
	if(isInput == 1 && isOutput == 1)
		isBoth = 1;
	else
		isBoth = 0;
	
	//printf("tokens->items[tokens->size-1] = %s\n", tokens->items[tokens->size-1]);

	if(strcmp(tokens->items[tokens->size-1], "&") == 0)
	{
		isBackground = 1;
		printf("& was found\n");
		k_count = 0;
		for(i_count = 0; i_count < 10; i_count++)
		{
			printf("i_count is now: %d\n", i_count);
			if(bg_process[i_count] == -1)		//make sure to reset when a pid is finished
			{
				while(tokens->items[k_count] != NULL)
				{
					if (k_count == 0)
						strcpy(bg_commands[i_count], tokens->items[k_count]);
					else
						strcat(bg_commands[i_count], tokens->items[k_count]);

					strcat(bg_commands[i_count], " ");
					k_count++;
				}
				break;
			}
		}

		for(int i = 0; i < tokens->size-1; i++)
		{
			add_token(temp_bg, tokens->items[i]);
			//printf("temp_bg_token(%d) - isBackgroundfound = %s\n", i, temp_bg->items[i]);
		}
		//printf("i_count is now: %d\n", i_count);
	}
	

	//<------------Background processing logic end ------------> 

	pid_t pid = fork();

	if(isBackground == 1)
	{
		bg_process[i_count] = pid;
		pid_t status = waitpid(pid, NULL, WNOHANG);
		//printf("bg_process[%d] is %d\n", i_count, pid);
	}

	if (pid == 0)		//for some reason, this is getting called more then once occassionally
	{	
		if(isInput == 1)
		{	
			close(STDIN_FILENO);
			dup(fd_in);
			close(fd_in);
			execv(temp_io->items[0], temp_io->items);
		}
		if(isOutput == 1)
		{ 
			close(STDOUT_FILENO);
			dup(fd_out);
			close(fd_out);
			execv(temp_io->items[0], temp_io->items);
		}
		if(isBackground == 0 && isBoth == 0)
		{
			execv(tokens->items[0], tokens->items);		//replace [0] in tokens with the cmdpath
		}
		else if(isBackground == 1 && isBoth == 0)
		{
			execv(temp_bg->items[0], temp_bg->items);
		}	
		exit(1);
	}
	else
	{
		free_tokens(temp_io);

		if(isOutput == 1)
			close(fd_out);
		if(isInput == 1)
			close(fd_in);
		
		if(isBackground == 0)
			waitpid(pid, NULL, 0);
		/*else
			waitpid(pid, NULL, WNOHANG);*/

		printf("Child exited\n");
	}
	
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






