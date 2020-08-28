#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main()
{
	while (1) {
		printf("> ");

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

        printf("Temp is: %s\n", tempStr);
        printf("Tempsize is: %d\n", strlen(tempStr));
        //while(i < tokens->size)
          //temp[i-1] = tokens->items[i];

        printf("Hi\n");
  		}
		}

		/*for (int i = 0; i < tokens->size; i++) {
			if(*(tokens->items[i]) == '$'){
				while(*(tokens->items[i]) != ' ' ){
					envVar += *(tokens->items[i])
				}
			}
		}

		if(*(tokens->items[0]) == '$'){
			printf("Hi\n");
		}*/

		free(input);
		free_tokens(tokens);

	}

	return 0;
}
