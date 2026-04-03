#include  <stdio.h>
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
E_table e_table;

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

    initialize_process_table(&p_table);


    if (signal(SIGINT, signal_handler) == SIG_ERR) {
            printf("Error setting signal handler\n");
    }

    // not interactive mode
    if(!isatty(STDIN_FILENO)){
        while(getline(&input, &input_size, stdin) != -1){
            token_list = tokenize(input, &num_tokens);
            int p = parse(token_list, &num_tokens, &command_list);

            if(p){
                //display_commands(command_list);
                int r = run_process(command_list);
                if(r == -1){
                    stop = 1;
                }
            }
        }
    }else{
        //interactive mode
        while(!stop){
           if(getcwd(cwd, sizeof(cwd)) == NULL){
               printf("Error getting current working directory\n");
               exit(1);
           }
    
           //get input from user
           printf("[%s]> ", cwd);
           input = readline("");
    
           if(input == NULL || input[0] == '\0' || input[0] == '\n'){
               continue;
           }
    
           add_history(input);
           
           // monitor_process(&p_table);
           //
           if(strcmp(input, "exit") == 0){
               stop = 1;
               continue;
           }
    
           token_list = tokenize(input, &num_tokens);
    
            for(int i = 0; i  <= num_tokens; i++){
               printf("[%s] type: %d\n", token_list[i].str, token_list[i].type);
            }
    
           int p = parse(token_list, &num_tokens, &command_list);
    
           if(p){
               //display_commands(command_list);
                int e = set_env_var();
                if(e) printf("successfully set env variable\n");
               int r = run_process(command_list);
               if(r == -1){
                   stop = 1;
               }
           }
       }
    }
    
    free(input);
    free(token_list);
    destroy_process_table(&p_table);
    return 0;
}
