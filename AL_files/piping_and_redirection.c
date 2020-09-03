#include <unistd.h>
/* PIPING SCHEME
    stdin -> 0 -> 0 -> 0 -> stdout 
*/

static int piping(int input, int first, int last)
{
    int pipettes[2];

    pipe(pipettes); //Invoke pipe
    pid = fork();

    if (pid == 0)
    {
        if (first == 1 && last == 0 && input == 0) //CMD1
        { 
            dup2(pipettes[WRITE], STDOUT_FILENO);
        }
        else if (first == 0 && last == 0 && input != 0) //CMD2
        { 
            dup2(input, STDIN_FILENO);
            dup2(pipettes[WRITE], STDOUT_FILENO);
        }
        else //CMD3
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
