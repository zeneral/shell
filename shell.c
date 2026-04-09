#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

#define WRITE_END 1
#define READ_END 0

int change_directory(char *dir){
    if(chdir(dir) == -1){
        printf("Error changing directory\n");
        return 0;
    }
    return 1;
}

int jobtype(char **tokens, int *num_tokens){
    for(int i = 0; tokens[*num_tokens - 1][i] != '\0'; i++){
        if(tokens[*num_tokens - 1][i] == '&'){
            printf("This is background process\n");
            tokens[*num_tokens - 1][i] = '\0';
            return 1;
        }
    }
    return 0;
}


int set_env_var(){
    if(e_table.count < 1)
        return 0;
    
    for(int i = 1; i <= e_table.count; i++){
        if(e_table.table[i].name != NULL && e_table.table[i].value != NULL){
            setenv(e_table.table[i].name, e_table.table[i].value, 1);
        }else{
            printf("Unable to set env variable\n");
            return 0;
        }
    }
   return 1; 
}


int run_process(Command *command_list){
    if(command_list == NULL){
        return 1;
    }
    int pid,status;

    //       if user want to exit  
    if(strcmp(command_list->argv[0], "exit") == 0){
           printf("Bye!\n");
           return -1;
   }
        
        //       if user want to change directory
    if(strcmp(command_list->argv[0], "cd") == 0){
         if(command_list->argc == 1){
              change_directory(command_list->argv[1]);
         }
         return 1;
    }

    int prevfd = -1;

    int output_file = -1, input_file = -1;
    while(command_list  != NULL){
        // fd[0] is used to read from pipe
        // fd[1] is used to write to pipe

        int fd[2];
        // if this is not last command in the list the create pipe
        if(command_list->next != NULL){
            if(pipe(fd) == -1){
                perror("pipe");
                return 0;
            }
        }

        //fork a child process
        child_pid = fork();

        // Inside of child prcess
        if(child_pid == 0){
            // if there is previous process then close its input file descriptor
            if(command_list->output_file != NULL){
                output_file = open(command_list->output_file, O_WRONLY | O_CREAT, 0644);
                if(output_file == -1){
                    perror("open");
                    return 0;
                }

                if(dup2(output_file, STDOUT_FILENO) == -1){
                    perror("dup2");
                    return 0;
                }
                close(output_file);
            }

            if(prevfd != -1){
                if(dup2(prevfd, STDIN_FILENO) == -1){
                    perror("dup2");
                    return 0;
                }
                close(prevfd);
            }

            if(command_list->next != NULL){
                // dup2 is used to copy file descriptor form output of current
                // process to input of current process
                if(dup2(fd[WRITE_END], STDOUT_FILENO) == -1){
                    perror("dup2");
                    return 0;
                }
                // close the input file descriptor of current process
                close(fd[WRITE_END]);
                close(fd[READ_END]);
                // not closing the read end of pipe because it will be used in next iteration
            }

            int e = execvp(command_list->argv[0], command_list->argv);
            if (e == -1) {
                perror("execvp");
                return 0;
            }

        }else if(child_pid < 0){
                perror("fork");
                return 0;
        }else {
            if(command_list->jobtype == FG){
                waitpid(child_pid, &status, 0);
            }else {
                child_pid = -1;
            }
        }

        if(prevfd != -1){
            close(prevfd);
        }

        // end of child process

        if(command_list->next != NULL){
            // close the output file descriptor of current process
            close(fd[1]);
            // mark the input file descriptor of current process as previous
            // so it can be used in next iteration
            prevfd = fd[READ_END];
        }        
        command_list = command_list->next;
    }
    return 1;
}

void free_command_list(Command **command_list){
    while(*command_list != NULL){
        Command *temp = *command_list;
        *command_list = (*command_list)->next;
        free(temp->argv);
        free(temp);
    }
}

void free_env_table(){
    free(e_table.table);
    e_table.count = 0;
    e_table.size = 0;
}
