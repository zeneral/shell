#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


#define MAX_DIRECTORY_LENGTH 1024

int main(int argc, char *argv[])
{
    char *input = NULL;
    size_t input_size = 10;
    char *tokens[10];
    char command_history[10][10];
    int num_tokens, command_history_index = 0;

    char cwd[MAX_DIRECTORY_LENGTH];
    
    int pid,status, stop = 0;
    while(!stop){
        if(getcwd(cwd, sizeof(cwd)) == NULL){
            printf("Error getting current working directory\n");
            exit(1);
        }

        //get input from user
        printf("[%s]> ", cwd);
        getline(&input, &input_size, stdin);

        if(input[0] == '\0'){
            command_history_index = command_history_index % 10;
            int i;
            for(i = 0; input[i] != '\0'; i++){
                command_history[command_history_index][i] = input[i];
            }
            command_history[command_history_index][i] = '\0';
            command_history_index++;
        }


        tokenize(input, tokens, &num_tokens);

        if(strcmp(tokens[0], "history") == 0){
            save_history(command_history, input, &command_history_index);
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
