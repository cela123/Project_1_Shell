# Project_1_Shell
COP4610 Project 1 - Shell Implementation

Team Members: Alec Amico, Aria Delmar, Andrew Lindsay

Division of Labor:
    Parts 1-4, Alec and Aria
    Part 5, Alec and Aria
    Part 6, Alec
    Part 7, Alec, Aria, and Andrew
    Part 8, Aria, Andrew
    Part 9, Alec
    Part 10, Aria, Alec

Contents of tar archive:
    makefile - compiles all of the program files into a single executable "shell". 

    shell.c - Prompts the user for new commands/input from the user and calls the appropriate functions based on the
              the user's input. Shell.c also keeps track of the number of background processes and the total number
              of commands executed since the program started. 

    parser.h - Header file for parser.c

    parser.c - Implementation of parsing functions that allow for input to be easily
               tokenized. 

    path_search.h - Header file for path_search.c

    path_search.c - Includes the implementation of search_for_command(), execute_command(), and does_command_exist(). 
                    Function search_for_command() is called when a command is not a built in and when the path for the
                    command is not explicitly stated with the inclusion of '/'. The input from the user is passed to 
                    search_for_command() where the function searches the directories listed in the environement variable
                    $PATH for the speicified function. If it is found, execute_command() is called. If not, the command
                    is invalid. 
                    The function execute_command() not only executes a command found on the path, but also changes the 
                    file descriptors if necessary when I/O direction is present. Commands are executed by cloning the
                    running process, running the command on the child process, waiting for the command to finish, and 
                    then killing the child process once it is complete. 
                    The function does_command_exist() is how search_for_command() is able to know if a command exists on
                    the $PATH or not. It checks each of the directories from $PATH for the specified command and returns
                    1 or 0 accordingly. 

    built_ins.h - Header file for built_ins.c

    built_ins.c - Implements the functions exit, echo, cd, and jobs. 

    piping_and_redirection.h - Header file for piping_and_redirection.c

    piping_and_redirection.c - Implements execution of piping. Checks for if one or two pipes are present in input,
                               tokenizes the commands accordingly, then calls search_for_command() to find an execute the
                               commands.  

How to compile executables with Makefile
    When the makefile is run with "make" the executable "shell" will be created. The executable will launch the program. 

Known bugs/unfinished project parts:
    Piping only works with one pipe. 
    There is a warning for implicit declaration of function ‘setenv’ that we could not resolve. 

Extra credit documentation:
    Shell-ception - the shell can be executed within the running shell process.
    Piping (though with only one pipe) works with IO redirection. 

Note: Andrew joined the team after Alec and Aria had begun work, which is why Andrew's contributions 
      are not noted until part 7. 

