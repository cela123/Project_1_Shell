//#include "parser.h"
//Part 5) $PATH search
//when a command is not starting with / or built-in
//search for the command within $PATH
//if not within $PATH then display an error message command not found

void search_for_command (char*, tokenlist*);
void execute_command(char*, tokenlist* );
int does_command_exist(char*);

//PART 7) INPUT REDIRECITON

int fd; // file descriptor 
int len; // length variable for various strings
static int flag_pipe = 1;
static int num_pipes = 0; // pipe counter
int line_count; //line counter
int output_redirection, input_redirection; // binary flag used as boolean for whether the input command string contains '>', '<', or both. 

int pid;


char buffer[1024]; // write buffer
char cdir[1000]; //current directory path
char write_to_file[3000]; // contents written
char *input_file, *output_file;
char *io_exec[100]; // IO actions to execute

static char* skip_white_space(char*); // helper function for redirection
static char* args[512]; // commands

void tokenize_redirect_io(char *); // cleans up string for input and output redirection,  with both '>' and '<'
void tokenize_redirect_i(char *); // cleans up string for input redirection, with '<'
void tokenize_redirect_o(char *); // cleans up string for output redirection, with '>'
void tokenize_coms(char*); // cleans up args
void write_file(); // writes to file
void clear(); // clears IO vars
