
//COP4610 Project 1 - Shell
//file: piping_and_redirection.h
// PIPING SCHEME: stdin -> 0 -> 0 -> 0 -> stdout 

#ifndef PIPING_AND_REDIRECTION
#define PIPING_AND_REDIRECTION

static int piping_and_redirection(int, int, int);
static char* args[512]; //Char array to store arguments

#endif 

/*

Part 7: I/O Redirection [10]

Now that your shell can execute external commands, we are going to build up more functionalities
around it in parts 7, 8, and 9. These functionalities only apply to external commands. In other words,
you do not need to implement them for built-in functions (part 10).

In this section, we are going to implement I/O redirection from/to a file. Normally, the shell receives
input from the keyboard and writes outputs to screen. Input redirection (<) replaces the keyboard
and output redirection (>) replaces the screen with a specified file.

I/O redirection should behave in the following manner
● CMD > FILE
    ○ CMD writes its standard output to FILE
    ○ Create FILE if it does not exist
    ○ Overwrite FILE if it does exist
● CMD < FILE
    ○ CMD receives its standard input from FILE
    ○ Signal an error if FILE does not exist or is not a file
● CMD < FILE_IN > FILE_OUT and CMD > FILE_OUT < FILE_IN
    ○ Follows the rules specified above
*/

/*

Part 8: Piping [10]

The second functionality we are going to add to external command execution is piping. In a way,
piping is an advanced form of I/O redirection. A difference to note is that piping requires more than 1
command. Instead of redirecting the I/O with a file, piping redirects the output of the first command
with the input of the second command. For this project, there will only be a maximum of 2 pipes in
any one command.
Piping should behave in the following manner
● CMD1 | CMD2
    ○ CMD1 redirects its standard output to CMD2's standard input
● CMD1 | CMD2 | CMD3
    ○ CMD1 redirects its standard output to CMD2's standard input
    ○ CMD2 redirects its standard output to CMD3's standard input
*/
