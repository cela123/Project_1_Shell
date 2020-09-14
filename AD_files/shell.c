#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/types.h"
#include "parser.h"
#include "built_ins.h"
#include "path_search.h"


int has_slash(char*); 
int has_IO(char*);

int main()
{
	int numCommands = 0; 
	pid_t bg_process[10];
	char * bg_commands[10];

	for(int i = 0; i < 10; i++)
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
				printf("[%d] %d\n", ++count, bg_process[i]);
			}
		}

		//prompt format USER@MACHINE : PWD > (part 3)
		printf("%s@%s:%s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		//collects everything entered by user into input
		char *input = get_input();

		//separates tokens (by spaces)
		tokenlist *tokens = get_tokens(input);

		//checks for IO for every input so it auto updates in calls to exec and search
		int hasIO = has_IO(input); 
		//Check in case there are no tokens
		if(tokens->size == 0){}

		else{
	
			for (int i = 0; i < tokens->size; i++) 
			{		
				//dereference env vars (part2)
      			if(*(tokens->items[i]) == '$')
				{
					char * temp = getenv(&(tokens->items[i][1]));
					if (temp == NULL)
						temp = "";

					char * var = (char*)malloc(strlen( temp ) + 1);
					strcpy(var, temp);

					free(tokens->items[i]);	
					
					tokens->items[i] = var; 	
/* 					char * var = (char*)malloc(strlen( &(tokens->items[i][1]) ));
					var = getenv(&(tokens->items[i][1]));
					//char *var = getenv(&(tokens->items[i][1])); 
					printf("token %d: (%s)\n", i, tokens->items[i]);
					printf("var: %s\n", var);

					if(var==NULL){	//if env var dne, replace token with empty string		
						free(tokens->items[i]);				
						tokens->items[i] = ""; 
					}
					else{		
						//strcpy(tokens->items[i], var);
						//if env var does exist replace token with var						
						tokens->items[i] = var; 							//CURRENTLY DOES NOT WORK
					}
					printf("token %d: (%s)\n", i, tokens->items[i]);	 */		

  				}
			
				else if(tokens->items[i][0] == '~')
				{
					char * temp = &(tokens->items[i][1]);
					char * home = getenv("HOME");

					int totalStrlen = strlen(home) + strlen(temp) + 1;

					char * var = (char*)malloc(totalStrlen);
					strcpy(var, home);
					strcat(var, temp);

					free(tokens->items[i]);
					tokens->items[i] = var;

					//printf("tokens->items[%d] is %s\n", i, tokens->items[i]);
					if(strlen(temp) <= 0 && i <= 0)
						printf("bash: %s: Is a directory\n", tokens->items[i]);
				}
				
	 
			}
			//checking if input is a built-in command and executing if it is

			/* if(isEnv == 0)
			{ */
				if(strcmp(tokens->items[0], "exit")==0)
				{
					printf("executing built-in exit\n"); 
					exit(numCommands); 
				}
				else if(strcmp(tokens->items[0], "cd")==0){
					printf("executing built-in cd\n"); 
					if(tokens->items[2] != NULL)	//case for too many arguments
						printf("error: too many arguments\n"); 
					else{
						
						cd(tokens->items[1]); 
					}
				}	
				else if(strcmp(tokens->items[0], "echo")==0){
					printf("executing built-in echo\n"); 
					echo(tokens); 
				}		
				else if(strcmp(tokens->items[0], "jobs")==0){
					printf("executing built-in jobs\n"); 
				}
			//checks for '/' in user input and executes given input
				else if(has_slash(tokens->items[0]) == 1){
					printf("user input has slashes\n");
					execute_command(tokens->items[0], tokens, 0); 
				}
				else{
					search_for_command(tokens->items[0], tokens);
				}	
			/* } */
			

			for(int i =0; i<tokens->size; i++)
				printf("token %d: %s\n", i, tokens->items[i]); 
		
		
		free(input);
		free_tokens(tokens);

		}



	}

	return 0;
}

//returns 1 if command has a slash, returns 0 if it doesn't
int has_slash(char* command)
{
	int ret = 0; 
	for(int i=0; i<strlen(command); i++){
		if(command[i] == '/')
			ret = 1; 
	}

	return ret; 
}

//returns 1 if input has > or <, returns 0 if it doesn't
int has_IO(char* input){
	int ret = 0; 
	for(int i=0; i<strlen(input); i++){
		if(input[i] == '>' || input[i] == '<')
			ret = 1; 
	}

	return ret; 
}