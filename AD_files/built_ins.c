
#include "parser.h"
#include "built_ins.h"

#include <sys/wait.h>
#include <stdio.h>





void echo(tokenlist* input){
    for(int i = 1; i < input->size; i++){
        printf("%s ", input->items[i]);
        
    }
    printf("\n"); 
}