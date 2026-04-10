#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "shell.h"


#define MAX_DIRECTORY_LENGTH 1024


pid_t child_pid = -1;

E_table e_table = {
    .table = NULL,
    .count = 0,
    .size = 0
};

void signal_handler(int sig){
    if(child_pid != -1){
        kill(child_pid, SIGKILL);
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    char *input = NULL;
    size_t input_size = 10;
    char *tokens[10];
    int num_tokens; 
    Token *token_list = NULL;
    Command *command_list = NULL;
    char cwd[MAX_DIRECTORY_LENGTH];
    
    int job_type, pid,status, stop = 0;



    if (signal(SIGINT, signal_handler) == SIG_ERR) {
            printf("Error setting signal handler\n");
    }

    // not interactive mode
    if(!isatty(STDIN_FILENO)){
        while(getline(&input, &input_size, stdin) != -1){
            token_list = tokenize(input, &num_tokens);
            #ifdef DEBUG_SHELL
                display_tokens(token_list, num_tokens); 
            #endif
            int p = parse(token_list, &num_tokens, &command_list);

            if(p){
                #ifdef DEBUG_SHELL
                    display_commands(command_list);
                #endif
                int r = run_process(command_list);
                if(r == -1){
                    stop = 1;
                }
            }
            free(token_list);
            free_command_list(&command_list);
            free_env_table();
            free(input);
            input = NULL;
            token_list = NULL;
        }
    }else{
        //interactive mode
        while(!stop){
           if(getcwd(cwd, sizeof(cwd)) == NULL){
               printf("Error getting current working directory\n");
               exit(1);
           }
    
           //get input from user
           printf("[%s]", cwd);
           input = readline(">");
    
           if(input == NULL || input[0] == '\0' || input[0] == '\n'){
               continue;
           }
    
           add_history(input);
           
           token_list = tokenize(input, &num_tokens);

            #ifdef DEBUG_SHELL
                display_tokens(token_list, num_tokens); 
            #endif
   
           int p = parse(token_list, &num_tokens, &command_list);
    
           if(p){
                #ifdef DEBUG_SHELL
                    display_commands(command_list);
                    display_env_table();
                #endif
                int e = set_env_var();
                if(e) printf("successfully set env variable\n");
               int r = run_process(command_list);
               if(r == -1){
                   stop = 1;
               }
           }
            free(input);
            free(token_list);
            free_command_list(&command_list);
            free_env_table();
       }
    }
    
    input = NULL;
    token_list = NULL;
    return 0;
}
