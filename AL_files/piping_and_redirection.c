#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
 
#include <sys/types.h>
#include <sys/wait.h>
#include "piping_and_redirection.h"

/*
 * Handle commands separately
 * input: return value from previous command (useful for pipe file descriptor)
 * first: 1 if first command in pipe-sequence (no input from previous pipe)
 * last: 1 if last command in pipe-sequence (no input from previous pipe)
 *
 * EXAMPLE: If you type "ls | grep shell | wc" in your shell:
 *    fd1 = piping(0, 1, 0), with args[0] = "ls"
 *    fd2 = piping(fd1, 0, 0), with args[0] = "grep" and args[1] = "shell"
 *    fd3 = piping(fd2, 0, 1), with args[0] = "wc"
 *
 * So if 'piping' returns a file descriptor, the next 'command' has this
 * descriptor as its 'input'.
 */


int piping(int input, int first, int last)
{
    int READ = 0, WRITE = 1;
    int pipettes[2];
    pid_t pid;
    pipe(pipettes); //Invoke pipe
    pid = fork();

    if (pid == 0)
    {
        if (first == 1 && last == 0 && input == 0)          //CMD1
        { 
            dup2(pipettes[WRITE], STDOUT_FILENO);
        }
        else if (first == 0 && last == 0 && input != 0)     //CMD2
        { 
            dup2(input, STDIN_FILENO);
            dup2(pipettes[WRITE], STDOUT_FILENO);
        }
        else                                                //CMD3
        { 
            dup2(input, STDIN_FILENO);//last command
        }

        if (execvp(args[0], args) == -1)
            _exit(EXIT_FAILURE); // If child fails
    }

    if (input != 0)
        close(input);

    // Done with stdout, so close write
    close(pipettes[WRITE]);

    // Last command, so done with stdin, close read
    if (last == 1)
        close(pipettes[READ]);

    return pipettes[READ];
}
