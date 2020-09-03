#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"

int has_slash(char*); 

int main()
{
	char *built_in_commands[] = {"exit", "cd", "echo", "jobs"};

//unnecessary bs i did bc c is dumb:
/* 	tokenlist *built_in = get_tokens(built_in_commands[0]);

	for(int i = 1; i <4; i++)
		add_token(built_in, built_in_commands[i]); 
		for (int i = 0; i < built_in->size; i++) {
			printf("token %d: (%s)\n", i, built_in->items[i]);
		} */

	while (1) 
	{
		//prompt format USER@MACHINE : PWD > (part 3)
		printf("%s@%s:%s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		//collects everything entered by user into input
		char *input = get_input();
		printf("whole input: %s\n", input);

		//separates tokens (by spaces)
		tokenlist *tokens = get_tokens(input);

		//prints out individual tokens 1 at a time
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}
		printf("number of tokens is %d \n\n", tokens->size); 


//CHECKING USER INPUT FOR COMMANDS, ENV VARIABLES, ETC
		for (int i = 0; i < tokens->size; i++) 
		{
			//search_for_path_command(tokens->items[i]);
			//printf("token %d: (%s)\n", i, tokens->items[i]);
			char tempStr[strlen(tokens->items[i])];
			//dereference env vars (part2)
      		if(*(tokens->items[i]) == '$')
			{
				strcpy(tempStr, tokens->items[i]);
				memmove(tempStr, tempStr+1, strlen(tempStr));
	        	printf("%s dereferenced: %s\n", tempStr, getenv(tempStr));
  			}
			
			else if(*(tokens->items[i]) == '~')
			{
				strcpy(tempStr, tokens->items[i]);
				memmove(tempStr, tempStr+1, strlen(tempStr));
				printf("%s%s\n", getenv("HOME"), tempStr);
			}
			//checking if input is a built-in command and executing if it is
			else if(strcmp(tokens->items[i], built_in_commands[0])==0){
				printf("executing built-in exit\n"); 
			}
			else if(strcmp(tokens->items[i], built_in_commands[1])==0){
				printf("executing built-in cd\n"); 
			}	
			else if(strcmp(tokens->items[i], built_in_commands[2])==0){
				printf("executing built-in echo\n"); 
			}		
			else if(strcmp(tokens->items[i], built_in_commands[3])==0){
				printf("executing built-in jobs\n"); 
			}
			//checks for '/' in user input and executes given input
			else if(has_slash(tokens->items[i]) == 1){
				printf("user input has slashes\n");
<<<<<<< HEAD
				execute_command(tokens->items[i], token->items); 
=======
				execute_command(tokens->items[i], tokens); 
>>>>>>> 78d039e8b14f7813729fcf545b529193b62cc1e9
			} 
			else{
				search_for_command(tokens->items[i], tokens); 
			}	

			
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}

//returns 1 if command has a slash, returns 0 if it doesn't
int has_slash(char* command){
	int ret = 0; 
	for(int i=0; i<strlen(command); i++){
		if(command[i] == '/')
			ret = 1; 
	}

	return ret; 
}