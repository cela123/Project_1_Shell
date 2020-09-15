#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "parser.h"
#include "built_ins.h"


void exit(int numCommands){
    waitpid(-1, NULL, 0);
}

void cd(char* path){

    if(path == NULL){ //if no arg is given for cd
        chdir(getenv("HOME")); 
        char* cwd = getcwd(NULL, 0); 
        setenv("PWD", cwd, 1); 
        free(cwd); 
    }
    else if(chdir(path) == -1)
        printf("error: invalid directory\n");
    else{
        char* cwd = getcwd(NULL, 0); 
        setenv("PWD", cwd, 1); 
        free(cwd); 
    }
}

void echo(tokenlist* input){
    for(int i = 1; i < input->size; i++){
        printf("%s ", input->items[i]);
        
    }
    printf("\n"); 
}