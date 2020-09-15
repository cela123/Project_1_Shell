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
	int breakCondition = 1;

	for(int i = 0; i < 10; i++)		//initialize background processing arrays
	{
		bg_process[i] = -1;
		bg_commands[i] = (char*)malloc(100);
		running_commands[i] = (char*)malloc(100);
	}

	while (breakCondition) 
	{
		int count = 0;

		for(int i = 0; i < 10; i++)
		{
			if(bg_process[i] != -1)
			{
				if(waitpid(bg_process[i], NULL, WNOHANG) != 0)
				{
					printf("[%d]+ Done \t\t %s\n", ++count, bg_commands[i]);
					bg_process[i] = -1;
					strcpy(bg_commands[i], "");
				}
				else
				{
					strcpy(running_commands[i], bg_commands[i]);
					//printf(" running_commands[%d] is %s\n", i, running_commands[i]);
					count++;
				}
			}
		}

		printf("totalCommands is: %d\n", totalCommands);
		
		//fix cases where getenv($ENVVAR) DNE to null / empty string
		printf("%s@%s : %s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		char *input = get_input();
		//printf("whole input: %s\n", input);
		tokenlist *tokens = get_tokens(input);

		if(tokens->size == 0){}	//skip checking tokens if token list is empty
		else
		{
			for (int i = 0; i < tokens->size; i++) 
			{
      			if(*(tokens->items[i]) == '$')
				{
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
				//printf("executing built-in exit\n");
				b_exit(totalCommands); 
			
			}
			else if(strcmp(tokens->items[0], "cd")==0){
				//printf("executing built-in cd\n");

				if(tokens->items[2] != NULL)	//case for too many arguments
					printf("error: too many arguments\n"); 
				else
					cd(tokens->items[1]);
					
				totalCommands++;
			}	
			else if(strcmp(tokens->items[0], "echo")==0){
				//printf("executing built-in echo\n");
				echo(tokens);
				totalCommands++;
			}		
			else if(strcmp(tokens->items[0], "jobs")==0){
				//printf("executing built-in jobs\n");
				jobs(bg_process, running_commands);
				totalCommands++;
			}
			else if(has_slash(tokens->items[0]) == 1){		//executing even if / is an env and not a path to a command
				//printf("user input has slashes\n");
				execute_command(tokens->items[0], tokens, 0, bg_process, bg_commands);
				totalCommands++;
			}
			else{											//defaulting is causing issues if first token is an env or ~
				search_for_command(tokens->items[0], tokens, bg_process, bg_commands);
				totalCommands++;
			}

			free(input);
			free_tokens(tokens);
		}
	}

	return 0;
}

int has_slash(char* command)
{
	int ret = 0; 
	for(int i=0; i<strlen(command); i++){
		if(command[i] == '/')
			ret = 1; 
	}

	return ret; 
}

