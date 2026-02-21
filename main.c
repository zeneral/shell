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
        printf("> ");
        getline(&input, &input_size, stdin);
        tokenize(input, tokens, &num_tokens);
        

       // if user want to exit  
        if(strcmp(tokens[0], "exit") == 0){
            printf("Bye!\n");
            stop = 1;
            break;
        }

       if (pid == 0) {
            int e = execvp(tokens[0], tokens);
            if (e == -1) {
                perror("execvp");
                exit(1);
            }
            printf("Child process exited\n");
            exit(0);
        }else if(pid < 0){
            perror("fork");
            exit(1);
        }else {
            wait(&status);
            if (WIFEXITED(status)) {
                printf("Child process exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }
    

    // for (int i = 0; i < num_tokens; i++) {
        // printf("%s\n", tokens[i]);
    // }
    free(input);
	return 0;
}
