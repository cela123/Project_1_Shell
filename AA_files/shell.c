#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parser.h"
#include "path_search.h"
#include "built_ins.h"

int has_slash(char*); 

int main()
{
	pid_t bg_process[10];
	char * bg_commands[10];
	char * running_commands[10];
	int totalCommands = 0;

	for(int i = 0; i < 10; i++)		//initialize background processing arrays
	{
		bg_process[i] = -1;
		bg_commands[i] = (char*)malloc(100);
		running_commands[i] = (char*)malloc(100);
	}

	while (1) 
	{
		int count = 0;

		for(int i = 0; i < 10; i++)
		{
			if(bg_process[i] != -1)
			{
				if(waitpid(bg_process[i], NULL, WNOHANG) != 0)
				{
					printf(" [%d]+ Done \t\t %s\n", ++count, bg_commands[i]);
					bg_process[i] = -1;		//reset array slots in function
					strcpy(bg_commands[i], "");
					strcpy(running_commands[i], "");
				}
				else	//if process is running copy into running_commands
				{
					strcpy(running_commands[i], bg_commands[i]);
					count++;
				}
			}
		}
		printf("%s@%s : %s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		char *input = get_input();
		tokenlist *tokens = get_tokens(input);

		if(tokens->size == 0){}	//skip checking tokens if token list is empty
		else
		{
			for (int i = 0; i < tokens->size; i++) 
			{
      			if(*(tokens->items[i]) == '$')
				{
					//appends env var to rest of command 
					char * temp = getenv(&(tokens->items[i][1]));
					if (temp == NULL)
						temp = "";

					char * var = (char*)malloc(strlen( temp ) + 1);
					strcpy(var, temp);

					free(tokens->items[i]);	
					
					tokens->items[i] = var;

					//if env var alone is entered
					if(tokens->items[i][0] == '/' && i <= 0)
						printf("bash: %s: is a directory\n", tokens->items[i]);
					if(tokens->items[i][0] != '/' && i <= 0)
						printf("bash: %s: command not found\n", tokens->items[i]);		
  				}
				if(*(tokens->items[i]) == '~')
				{	
					//appends HOME env var wherever ~ is found
					char * temp = &(tokens->items[i][1]);
					char * home = getenv("HOME");

					int totalStrlen = strlen(home) + strlen(temp) + 1;

					char * var = (char*)malloc(totalStrlen);
					strcpy(var, home);
					strcat(var, temp);

					free(tokens->items[i]);
					tokens->items[i] = var;

					//if ~ is entered alone
					if(strlen(temp) <= 0 && i <= 0)
						printf("bash: %s: Is a directory\n", tokens->items[i]);
				}		
			} 

			if(strcmp(tokens->items[0], "exit")==0){
				b_exit(totalCommands); 
			}
			else if(strcmp(tokens->items[0], "cd")==0){
				if(tokens->items[2] != NULL)	//case for too many arguments
					printf("error: too many arguments\n"); 
				else
					cd(tokens->items[1]);
					
				totalCommands++;
			}	
			else if(strcmp(tokens->items[0], "echo")==0){
				echo(tokens);
				totalCommands++;
			}		
			else if(strcmp(tokens->items[0], "jobs")==0){
				jobs(bg_process, running_commands);
				totalCommands++;
			}
			else if(has_slash(tokens->items[0]) == 1){		
				execute_command(tokens->items[0], tokens, 0, bg_process, bg_commands);
				totalCommands++;
			}
			else{											
				search_for_command(tokens->items[0], tokens, bg_process, bg_commands);
				totalCommands++;
			}

			free(input);
			free_tokens(tokens);
		}
	}

	return 0;
}

//determines whether not the input has a slash for executing commands based on path
int has_slash(char* command)
{
	int ret = 0; 
	for(int i=0; i<strlen(command); i++){
		if(command[i] == '/')
			ret = 1; 
	}

	return ret; 
}

