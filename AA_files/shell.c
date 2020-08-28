#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main()
{
	while (1) {
		printf("%s@%s : %s > ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);

      if(*(tokens->items[i]) == '$'){
        char tempStr[strlen(tokens->items[i])];

				strcpy(tempStr, tokens->items[i]);
			  if(tempStr[0] == '$')
					memmove(tempStr, tempStr+1, strlen(tempStr));

        printf("%s dereferenced: %s\n", tempStr, getenv(tempStr));
        printf("Tempsize is: %d\n", strlen(tempStr));


        printf("Hi\n");
  		}
		}

		free(input);
		free_tokens(tokens);

	}

	return 0;
}
