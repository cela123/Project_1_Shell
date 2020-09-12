#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"
#include "piping_and_redirection.h"

int has_slash(char*); 
int has_IO(char*);

int main()
{
	//char *built_in_commands[] = {"exit", "cd", "echo", "jobs"};

	while (1) 
	{
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
			
				//char * tempStr = (char*)malloc(strlen(tokens->items[i]));
				//dereference env vars (part2)
      			if(*(tokens->items[i]) == '$')
				{
					char *var = getenv(&(tokens->items[i][1])); 
					printf("token %d: (%s)\n", i, tokens->items[i]);


					free(tokens->items[i]); 

					if(var==NULL){	//if env var dne, replace token with empty string						
						tokens->items[i] = ""; 

					}
					else{		
						//if env var does exist replace token with var						
						tokens->items[i] = var; 							//CURRENTLY DOES NOT WORK
					}
					printf("token %d: (%s)\n", i, tokens->items[i]);			

  				}
			
				else if(*(tokens->items[i]) == '~')
				{
					char *home = getenv("HOME"); 
					free(tokens->items[i]); 
					tokens->items[i] =  home; 
					//strcpy(tempStr, tokens->items[i]);
					//memmove(tempStr, tempStr+1, strlen(tempStr));
					//printf("%s%s\n", getenv("HOME"), tempStr);
				}
	 
			}
			//checking if input is a built-in command and executing if it is
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
			//checks for '/' in user input and executes given input
			else if(has_slash(tokens->items[0]) == 1){
				printf("user input has slashes\n");
				execute_command(tokens->items[0], tokens, 0); 
			} 
			else{
				search_for_command(tokens->items[0], tokens);
			}	

			
		
		
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