#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "shell.h"


#define MAX_DIRECTORY_LENGTH 1024


pid_t child_pid = -1;

ProcessTable p_table;

void signal_handler(int sig){
    if(child_pid != -1){
        kill(child_pid, SIGKILL);
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    char *input = NULL;
    size_t input_size = 10;
    char *tokens[10];
    int num_tokens; 

    char cwd[MAX_DIRECTORY_LENGTH];
    
    int job_type, pid,status, stop = 0;

    initialize_process_table(&p_table);


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
        input = readline("");
        add_history(input);

        if(input == NULL || input[0] == '\0' || input[0] == '\n'){
            continue;
        }
        
        
        tokenize(input, tokens, &num_tokens);
    
        job_type = jobtype(tokens, &num_tokens);
                               
        if(run_process(tokens, &num_tokens, job_type) == -1){
            break;
        }

    }
   free(input);
	return 0;
}
