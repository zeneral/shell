#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

int main(int argc, char *argv[])
{
    char *input = NULL;
    size_t input_size = 10;
    char *tokens[10];
    int num_tokens;
    
    int pid,status, stop = 0;
    while(!stop){

        //get input from user
        printf("> ");
        getline(&input, &input_size, stdin);
        tokenize(input, tokens, &num_tokens);
        

//       if user want to exit  
        if(strcmp(tokens[0], "exit") == 0){
            printf("Bye!\n");
            stop = 1;
            break;
        }

        run_process(tokens, &num_tokens);
    }
    

   free(input);
	return 0;
}
