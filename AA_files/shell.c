#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parser.h"
#include "path_search.h"

int has_slash(char*); 

int main()
{
	pid_t bg_process[10];
	char * bg_commands[10];

	for(int i = 0; i < 10; i++)		//initialize background processing arrays
	{
		bg_process[i] = -1;
		bg_commands[i] = (char*)malloc(100);
	}

	while (1) 
	{
		int count = 0;
		for(int i = 0; i < 10; i++)
		{
			if(bg_process[i] != -1)
			{
				//printf("------ [%d] %d ------\n", ++count, bg_process[i]);
				if(kill(bg_process[i], 0) == 0)
					printf(" [%d] %d ------ is still running\n", ++count, bg_process[i]);
				else
					printf(" [%d]+ %d ------ finished\n", ++count, bg_process[i]);
			}
		}
		
		//fix cases where getenv($ENVVAR) DNE to null / empty string
		printf("%s@%s : %s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		char *input = get_input();
		//printf("whole input: %s\n", input);
		tokenlist *tokens = get_tokens(input);
		int firstTokenCheck = 0;

		if(tokens->size == 0){}	//skip checking tokens if token list is empty
		else
		{
			for (int i = 0; i < tokens->size; i++) 
			{
      			if(*(tokens->items[i]) == '$')
				{
					if(i == 0)
						firstTokenCheck = 1;

					char * var = (char*)malloc(strlen( &(tokens->items[i][1]) ));
					var = getenv(&(tokens->items[i][1]));
					//printf("token %d: (%s)\n", i, tokens->items[i]);
					//printf("var: %s\n", var);

					if(var==NULL){	//if env var dne, replace token with empty string
						printf("if NULL -- (before free) token->items[%d] : (%s)\n", i, tokens->items[i]);		
						free(tokens->items[i]);				
						tokens->items[i] = ""; 
					}
					else{		
						printf("else -- (before) token->items[%d] : (%s)\n", i, tokens->items[i]);
						tokens->items[i] = var;
						printf("else -- (after) token->items[%d] : (%s)\n", i, tokens->items[i]);
					}
					//printf("token %d: (%s)\n", i, tokens->items[i]);			
  				}
				else if(*(tokens->items[i]) == '~')
				{
					if(i == 0)
						firstTokenCheck = 1;

					char *home = getenv("HOME"); 
					free(tokens->items[i]);
					tokens->items[i] = home; 
					//printf("home: %s\n", home);
					//printf("tokens->items[%d]: %s\n", i, tokens->items[i]);
				}
				//printf("inside for loop\n");		
			} //printf("outside for loop\n");

			printf("tokens->items[0]: %s\n", tokens->items[0]);
			printf("firstTokenCheck: %d\n", firstTokenCheck);

			if(strcmp(tokens->items[0], "exit")==0){
				printf("executing built-in exit\n"); 
			}
			else if(strcmp(tokens->items[0], "cd")==0){
				printf("executing built-in cd\n"); 
			}	
			else if(strcmp(tokens->items[0], "echo")==0){
				printf("executing built-in echo\n"); 
			}		
			else if(strcmp(tokens->items[0], "jobs")==0){
				printf("executing built-in jobs\n"); 
			}
			else if(has_slash(tokens->items[0]) == 1){		//executing even if / is an env and not a path to a command
				printf("user input has slashes\n");
				execute_command(tokens->items[0], tokens, 0, bg_process, bg_commands); 
			}
			else{											//defaulting is causing issues if first token is an env or ~
				search_for_command(tokens->items[0], tokens, bg_process, bg_commands);
			}

			/*printf("tokens->items[0] is %s\n", tokens->items[0]);
			printf("tokens->items[1] is %s\n", tokens->items[1]);
			printf("tokens->items[2] is %s\n", tokens->items[2]);
			printf("tokens->size-1 is %d\n", tokens->size-1);
			printf("tokens->size is %d\n", tokens->size);*/
			

			for(int i = 0; i < tokens->size+1; i++)
				printf("tokens->items[%d] is %s\n", i, tokens->items[i]);

			printf("before free calls\n");
			free(input);
			printf("between free calls\n");
			free_tokens(tokens);
			printf("after free calls\n");
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

