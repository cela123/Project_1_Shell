#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"

int main()
{
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
		
		//fix cases where getenv($ENVVAR) DNE to null / empty string
		printf("%s@%s : %s > ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) 
		{
			//printf("token %d: (%s)\n", i, tokens->items[i]);
			char tempStr[strlen(tokens->items[i])];		//dynamically allocated instead!!!!!!!!!!
      		if(*(tokens->items[i]) == '$')		//little hack for 2/4 : eos defined by null and the variable (token[i]) defines start of string 
			{
				strcpy(tempStr, tokens->items[i]);
				memmove(tempStr, tempStr+1, strlen(tempStr));		//&tokens->items[i][1] instead 
	        	printf("%s dereferenced: %s\n", tempStr, getenv(tempStr));		//printing null will crash (ie check if null) resolve to empty string and replace token in tokenlist (same as execute)
  			}
			if(*(tokens->items[i]) == '~')			//^--- replace token instead of just printing, you may have to do extra token operations in the future
			{
				strcpy(tempStr, tokens->items[i]);
				memmove(tempStr, tempStr+1, strlen(tempStr));
				printf("%s%s\n", getenv("HOME"), tempStr);
			}
			//checking if input is a built-in command and executing if it is		
		}

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
		else{	//first token is command, and tokens after pipe
			search_for_command(tokens->items[0], tokens, bg_process, bg_commands);
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}

