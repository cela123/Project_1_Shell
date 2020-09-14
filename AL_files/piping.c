#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "path_search.h"

char* strsep (char **cmdStr, const char *delim)
{
  char *begin, *end;
  begin = *cmdStr;

  if (begin == NULL)
    return NULL;

  // Find end of token
  end = begin + strcspn(begin, delim);

  if(*end)
  {
    //terminate the token and set cmdStr after null char
    *end++ = '\0';
    *cmdStr = end;
  }
  else //no more delimiters, this is last token
    *cmdStr = NULL;

  return begin;
}



tokenlist** partitionPipes(tokenlist* cmdStr) 
{
	char** cmdStrPiped;
	int i;
	for (i = 0; i < 2; i++) {
	  cmdStrPiped[i] = strsep((char*)&cmdStr, "|");
		
		if(cmdStrPiped[i] == NULL)
			break;
		printf(cmdStrPiped[i]);
	}
	
	return ((tokenlist**)cmdStrPiped);
}

