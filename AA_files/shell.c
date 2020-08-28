#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

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
			printf("token %d: (%s)\n", i, tokens->items[i]);
			char tempStr[strlen(tokens->items[i])];
      		if(*(tokens->items[i]) == '$')
			{
        		//char tempStr[strlen(tokens->items[i])];
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
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}
