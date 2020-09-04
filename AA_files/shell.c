#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"

int main()
{
	while (1) 
	{
		printf("%s@%s : %s > ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) 
		{
			//printf("token %d: (%s)\n", i, tokens->items[i]);
			char tempStr[strlen(tokens->items[i])];
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
			else{
				search_for_command(tokens->items[i], tokens);
			}

			
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}

