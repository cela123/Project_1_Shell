#include "piping_and_redirection.h"
#include "path_search.h"
#include "sys/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int hasPipe(char * command){
    int ret = 0; 
	for(int i=0; i<strlen(command); i++){
		if(command[i] == '|')
			ret = 1; 
	}
    return ret; 
}


void piping(char* command, tokenlist* input, pid_t* bg_process, char** bg_commands){
    int pipe1_index = 0; 
    int pipe2_index = 0; 
    int first_pipe = -1; 
    int numPipes; 
    int cmd1_index = 0; 
    int cmd2_index = -1; 
    int cmd3_index = -1; 
    tokenlist * cmd1 = new_tokenlist(); 
    tokenlist * cmd2 = new_tokenlist(); 
    tokenlist * cmd3 = new_tokenlist(); 
    

    for(int i = 0; i < input->size-1; i++){
        if(strcmp(input->items[i], "|") == 0){
            if(first_pipe = -1){
                pipe1_index = i; 
                first_pipe = i; 
                numPipes = 1; 
                cmd2_index = i+1; 
            }
            else{
                    pipe2_index = i; 
                    numPipes = 2; 
                    cmd3_index = i+1; 
            }
            
        }

    }
    printf("time to tokenize cmd1\n"); 

    //int cmd1_size = strlen(input->items[0]); 
    for(int i = 0; i < first_pipe; i++){
        add_token(cmd1, input->items[i]); 
    } 
    printf("time to tokenize cmd2\n"); 
    
    int j = cmd2_index; 
    if(numPipes == 1){
        while(input->items[j] != NULL)
            add_token(cmd2, input->items[j++]);
    }
    else if(numPipes == 2){
        while(j != pipe2_index)
            add_token(cmd2, input->items[j++]);
        
        j = cmd3_index; 
        while(input->items[j] != NULL)
            add_token(cmd3, input->items[j++]);
    }



    
    for(int i = 0; i < cmd1->size; i ++)
        printf("cmd1 token %d: %s\n",i, cmd1->items[i]); 

    for(int i = 0; i < cmd2->size; i ++)
        printf("cmd2 token %d: %s\n",i, cmd2->items[i]); 

    if(numPipes == 1){
        int p_fds[2];
        pipe(p_fds); 

        int pid1 = fork();

        if(pid1 == 0){
            close(p_fds[0]); 
            //redirect stdout to be input of pipe
            close(STDOUT_FILENO); 
            dup(p_fds[1]); 
            close(p_fds[1]); 
            search_for_command(cmd1->items[0], cmd1, bg_process, bg_commands);
            //execv(input->items[0], cmd1);
            exit(1); 
        }

        int pid2 = fork();

        if(pid2 == 0){
            close(p_fds[1]); 
            //redirect stdin to be output of pipe
            close(STDIN_FILENO);
            dup(p_fds[0]); 
            close(p_fds[0]); 
            search_for_command(cmd2->items[0], cmd2, bg_process, bg_commands);
            exit(1); 
        }

        close(p_fds[0]); 
        close(p_fds[1]);
        waitpid(pid1, NULL, 0);  
        waitpid(pid2, NULL, 0);  
    }
    if(numPipes == 2){
        int p_fds1[2];
        int p_fds2[2];
        pipe(p_fds1); 
        pipe(p_fds2); 

        int pid1 = fork();
        if(pid1 == 0){
            close(p_fds1[0]); 
            close(p_fds2[0]); 
            close(p_fds2[1]); 
            //redirect stdout to be input of pipe1
            close(STDOUT_FILENO); 
            dup(p_fds1[1]); 
            close(p_fds1[1]); 
            search_for_command(cmd1->items[0], cmd1, bg_process, bg_commands);
            //execv(input->items[0], cmd1);
            exit(1); 
        }    

        int pid2 = fork();
        if(pid2 == 0){ 
            close(p_fds1[1]); 
            close(p_fds2[0]); 
            //redirect stdin to be output of pipe1
            close(STDIN_FILENO);
            dup(p_fds1[0]); 
            close(p_fds1[0]); 
            //redirect stdout to be input of pipe2
            close(STDOUT_FILENO); 
            dup(p_fds2[1]); 
            close(p_fds2[1]); 
            search_for_command(cmd2->items[0], cmd2, bg_process, bg_commands);
            exit(1); 
        }

        int pid3 = fork();  
        if(pid3 == 0){
            close(p_fds1[0]); 
            close(p_fds1[1]); 
            close(p_fds2[1]); 
            //redirect stdin to be output of pipe2
            close(STDIN_FILENO);
            dup(p_fds2[0]); 
            close(p_fds2[0]); 
            search_for_command(cmd3->items[0], cmd3, bg_process, bg_commands);
            exit(1); 
        }
        close(p_fds1[0]); 
        close(p_fds1[1]);
        close(p_fds2[0]);
        close(p_fds2[1]);
        waitpid(pid1, NULL, 0);  
        waitpid(pid2, NULL, 0);  
        waitpid(pid3, NULL, 0);    

    }

}
