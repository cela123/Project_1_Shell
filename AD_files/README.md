# Project_1_Shell
COP4610 Project 1 - Shell Implementation

Team Members: Alec Amico, Aria Delmar, Andrew Lindsay

Division of Labor:
    Parts 1-4, Alec and Aria
    Part 5, Aria
    Part 6, Alec
    Part 7, Alec, Aria, and Andrew
    Part 8, Andrew
    Part 9, Alec
    Part 10, Aria

Contents of tar archive:
    shell.c - 

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

How to compile executables with Makefile
    When the makefile is run with "make" the executable "shell" will b

Known bugs/unfinished project parts:

Extra credit documentation:
    Shell-ception - the shell can executed within the running shell process

Note: Andrew joined the team after Alec and Aria had begun work, which is why Andrew's contributions 
      are not noted until part 7. 

