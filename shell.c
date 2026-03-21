#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

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

void monitor_process(ProcessTable *p_table){
    for(int i = 0; i < p_table->process_count; i++){
        int status;
        waitpid(p_table->table[i].pid, &status, WNOHANG); // WNOHANG is used to avoid zombie process
        if(WIFEXITED(status)){
            //delete_process(p_table, p_table->table[i].pid); // delete process from table
        }else if(WIFSTOPPED(status)){
            p_table->table[i].status = status; // update status of process in table
        }else if(WIFCONTINUED(status)){
            p_table->table[i].status = status; // upadate status of process in table
        }
    }
}


int run_process(Command *command_list){
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

    if(strcmp(command_list->argv[0], "jobs") == 0){
        display_process_table(&p_table);
        return 1;
    }

    int prevfd = -1;
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
        if(fork() == 0){
            // if there is previous process then close its input file descriptor
            if(prevfd != -1){
                dup2(prevfd, STDIN_FILENO);
                close(prevfd);
            }

            if(command_list->next != NULL){
                dup2(fd[1], STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);
            }

            int e = execvp(command_list->argv[0], command_list->argv);
            if (e == -1) {
                perror("execvp");
                return 0;
            }
        }

        if(prevfd != -1){
            close(prevfd);
        }

        if(command_list->next != NULL){
            close(fd[1]);
            prevfd = fd[0];
        }
        
    }

    

    if (child_pid == 0) {
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
            Process process = {
                .created_time = time(NULL),
                .pid = child_pid,
                .status = 0,
            };
            add_process(&p_table, &process);
        }
    }
    return 1;
}

