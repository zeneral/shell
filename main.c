#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "shell.h"


#define MAX_DIRECTORY_LENGTH 1024


void signal_handler(int sig){
    printf("Exiting...\n");
    int p = getpid();
    if(p > 0){
        kill(p, SIGKILL);
    }
}

int main(int argc, char *argv[])
{
    char *input = NULL;
    size_t input_size = 10;
    char *tokens[10];
    char command_history[10][10];
    int num_tokens, command_history_index = 0;

    char cwd[MAX_DIRECTORY_LENGTH];
    
    int pid,status, stop = 0;

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
            printf("Error setting signal handler\n");
    }

    while(!stop){
        if(getcwd(cwd, sizeof(cwd)) == NULL){
            printf("Error getting current working directory\n");
            exit(1);
        }

        //get input from user
        printf("[%s]> ", cwd);
        getline(&input, &input_size, stdin);

        if(input == NULL || input[0] == '\0' || input[0] == '\n'){
            continue;
        }
        
        if(input[0] != '\0'){
            save_history(command_history, input, &command_history_index);
        }


        tokenize(input, tokens, &num_tokens);

        if(strcmp(tokens[0], "history") == 0){
            for(int i = command_history_index - 1; i >= 0; i--){
                printf("[%d] %s", i,command_history[i]);
            }
            continue;
        }
                        

//       if user want to exit  
        if(strcmp(tokens[0], "exit") == 0){
            printf("Bye!\n");
            stop = 1;
            break;
        }
        
//       if user want to change directory

        if(strcmp(tokens[0], "cd") == 0){
            if(num_tokens == 2){
                change_directory(tokens[1]);
            }
            continue;
        }

        run_process(tokens, &num_tokens);
    }
    

   free(input);
	return 0;
}
