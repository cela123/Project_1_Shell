//Part 5) $PATH search
//when a command is not starting with / or built-in
//search for the command within $PATH
//if not within $PATH then display an error message command not found

void search_for_path_command(char*);
int path_size(char*);					//helper function for search_for_path_command
char** parse_path(char*);
