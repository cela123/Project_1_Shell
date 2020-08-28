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
        char temp[strlen(tokens->items[i])];
        printf("Temp is: %s\n", temp);
        printf("Tempsize is: %d\n", strlen(temp));
        //while(i < tokens->size)
          //temp[i-1] = tokens->items[i];

        printf("Hi\n");
  		}
		}

		/*if(*(tokens->items[0]) == '$'){
      int i = 1;
      char temp[strlen(tokens->items[i])];
      //while(i < tokens->size)
        //temp[i-1] = tokens->items[i];

      printf("Hi\n");
		}*/

		free(input);
		free_tokens(tokens);

	}

	return 0;
}
