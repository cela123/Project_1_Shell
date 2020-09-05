#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "parser.h"
#include "path_search.h"

void search_for_command(char *command, tokenlist *tokens)
{
    char *mainPATH = getenv("PATH");

    char *copyPATH = malloc(strlen(mainPATH));
    strcpy(copyPATH, mainPATH);

    char **parsedPATH = NULL;
    char *tok = strtok(copyPATH, ":");
    int size = 0;

    //allocates space in array for each individual path
    while (tok)
    {
        parsedPATH = realloc(parsedPATH, (sizeof(char *) * ++size));

        if (parsedPATH == NULL)
            exit(-1);

        parsedPATH[size - 1] = tok; //sets char pointer at size-1 to the token
        tok = strtok(NULL, ":");
    }

    //null-termination of array
    parsedPATH = realloc(parsedPATH, sizeof(char *) * size + 1);
    parsedPATH[size] = 0;

    //append command to a temp variable that is overwritten in each iteration to check then next path

    int i;
    for (i = 0; i < size; i++)
    {
        if (parsedPATH[i] != NULL)
        {
            int size_0 = strlen(parsedPATH[i]) + strlen(command) + 1;
            //char temp_command[size_0];		//needs to be dynamically allocated
            char *temp_command = (char *)malloc(size_0);
            strcpy(temp_command, parsedPATH[i]);
            strcat(temp_command, "/");
            strcat(temp_command, command);
            //printf("temp_command at %d: %s\n", i, temp_command);
            if (does_command_exist(temp_command))
            {
                printf("Running Command\n"); //insert command execution here
                execute_command(temp_command, tokens);
                printf("After running command\n");

                break;
            }
        }
    }

    if (size == i)
        printf("command not found\n");

    free(parsedPATH);
}


void execute_command(char *cmdpath, tokenlist *tokens)
{
    int mypipefd[2], ret, input_fd, output_fd;
    //free(tokens->items[0]);	//only if search_for_command is called first
    tokens->items[0] = cmdpath;

    for (int i = 0; i < tokens->size + 1; i++)
        printf("token %d: (%s)\n", i, tokens->items[i]);

    pid_t pid = fork();

    if (pid == 0) //for some reason, this is getting called more then once occassionally
    {
        execv(tokens->items[0], tokens->items); //replace [0] in tokens with the cmdpath
        
        if (strchr(cmdpath, '<') && strchr(cmdpath, '>'))
        {
            input_redirection = 1;
            output_redirection = 1;
            tokenize_redirect_io(tokens); // Tokenize redirection input AND output
        }
        else if (strchr(cmdpath, '<'))
        {
            input_redirection = 1;
            tokenize_redirect_i(tokens); // Tokenize redirection input
        }
        else if (strchr(cmdpath, '>'))
        {
            output_redirection = 1;
            tokenize_redirect_o(tokens); // Tokenize redirection output
        }

        if (output_redirection == 1)
        {
            output_fd = creat(output_file, 0644);
            if (output_fd < 0)
                fprintf(stderr, "Failed to open %s for writing\n", output_file);
            dup2(output_fd, 1);
            close(output_fd);
            output_redirection = 0;
        }

        if (input_redirection == 1)
        {
            input_fd = open(input_file, O_RDONLY, 0);
            if (input_fd < 0)
                fprintf(stderr, "Failed to open %s for reading\n", input_file);

            dup2(input_fd, 0);
            close(input_fd);
            input_redirection = 0;
        }
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("Child exited\n");
    } //need piping logic before and after the fork
}

// checks for exists of a command
int does_command_exist(char *path)
{
    FILE *check = fopen(path, "r");
    if (check == NULL)
        return 0;
    fclose(check);
    return 1;
}

// PARTS 7 & 8


void write_file()
{

    int fd_out, write_to_file, str_len = 0;
    char input[2000];
    line_count++;
    char no[10];
    sprintf(no, "%d", line_count);
    strcpy(input, " ");
    strcat(input, no);
    strcat(input, " ");
    strcat(input, buffer);

    str_len = strlen(input); // length of input string
    fd_out = open(buffer, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    len = strlen(buffer); // length of buffer after opening file
    write_to_file = write(fd_out, input, str_len); // output the source text as a file 

    if (write_to_file < 0)
    {
        printf("Error in writing file\n");
        return;
    }
    
    close(fd_out);
}

// Helper Functions for Parts 7 & 8

static char *skip_white_space(char *input_string)
{
    while (isspace(*input_string))
        ++input_string;

    return input_string;
}


void clear() // clear variables for I/O
{
    pid = 0; // reset pid
    line_count = 0; //reset line count
    num_pipes = 0; //reset num_pipes
    len = 0;
    flag_pipe = 0;
    fd = 0;
    output_redirection = 0;
    input_redirection = 0; // reset file descriptor & flags, & IO Redirection status (on or off)
    buffer[0] = '\0'; // clear write buffer
}

/* Tokenizers for Parts 7 & 8 */ 

//BOTH INPUT & OUTPUT REDIRECTION '<' & '>'
void tokenize_redirect_io(char *io_exec)
{
    int i = 1;
    char *io_token[100];
    char *new_io_exec = strdup(io_exec); 
    io_token[0] = strtok(new_io_exec, "<");

    while ((io_token[i] = strtok(NULL, ">")) != NULL)
        i++;
    
    io_token[1] = skip_white_space(io_token[1]);
    io_token[2] = skip_white_space(io_token[2]);

    input_file = strdup(io_token[1]);
    output_file = strdup(io_token[2]);

    tokenize_coms(io_token[0]);
}

//ONLY INPUT REDIRECTION '<'
void tokenize_redirect_i(char *io_exec)
{
    int i = 1;
    char *i_token[100]; //input token
    char *new_i_exec;
    new_i_exec = strdup(io_exec);
    i_token[0] = strtok(new_i_exec, "<");

    while ((i_token[i] = strtok(NULL, "<")) != NULL)
        i++;

    i_token[1] = skip_white_space(i_token[1]);
    input_file = strdup(i_token[1]);

    tokenize_coms(i_token[0]);
}

//ONLY OUTPUT REDIRECTION '>'
void tokenize_redirect_o(char *io_exec) 
{
    int i = 1;
    char *o_token[100]; //output token
    char *o_redirect; //create new C-String for redirection
    o_redirect = strdup(io_exec); //store io_exec
    o_token[0] = strtok(o_redirect, ">");

    while ((o_token[i] = strtok(NULL, ">")) != NULL)
        i++;

    o_token[1] = skip_white_space(o_token[1]);
    output_file = strdup(o_token[1]);

    tokenize_coms(o_token[0]);
}

// tokenize commands after redirection
void tokenize_coms(char *io_exec) 
{
    int i = 1;
    args[0] = strtok(io_exec, " ");
    
    while ((args[i] = strtok(NULL, " ")) != NULL)
        i++;
}
