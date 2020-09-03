#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "path_search.h"

int main()
{
	const char * built_in_commands[] = {"exit", "cd", "echo", "jobs"}; 	//think about how we're going to call part 5s
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
			search_for_command(tokens->items[i], tokens);
			
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}

