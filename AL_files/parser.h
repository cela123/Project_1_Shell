typedef struct {
	int size;
	char **items;
} tokenlist;

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
char *get_input(void);
tokenlist *get_tokens(char *input);
void free_tokens(tokenlist *tokens);

tokenlist** partitionPipes(tokenlist* cmdStr);
char* strsep(char** cmdStr, const char* delim);
