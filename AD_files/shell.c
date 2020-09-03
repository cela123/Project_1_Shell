#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"

int has_slash(char*); 

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

		//CHECKING USER INPUT FOR COMMANDS, ENV VARIABLES, ETC
		for (int i = 0; i < tokens->size; i++) 
		{
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
			else if(strcmp(tokens->items[i], "exit")==0){
				printf("executing built-in exit\n"); 
			}
			else if(strcmp(tokens->items[i], "cd")==0){
				printf("executing built-in cd\n"); 
			}	
			else if(strcmp(tokens->items[i], "echo")==0){
				printf("executing built-in echo\n"); 
			}		
			else if(strcmp(tokens->items[i], "jobs")==0){
				printf("executing built-in jobs\n"); 
			}
			//checks for '/' in user input and executes given input
			else if(has_slash(tokens->items[i]) == 1){
				printf("user input has slashes\n");
				execute_command(tokens->items[i], tokens); 
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