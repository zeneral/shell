#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


int tokenize(char *str, char **tokens, int *num_tokens){
	int i = 0;
	char *token;
	char *saveptr;

	token = strtok_r(str, " \n", &saveptr);
	while (token != NULL) {
		tokens[i] = token;
		i++;
		token = strtok_r(NULL, " \n", &saveptr);
	}
	*num_tokens = i;
    tokens[i++] = NULL;
	return 0;
}

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


int run_process(char **tokens, int *num_tokens, int job_type){
    int pid,status;

    //       if user want to exit  
    if(strcmp(tokens[0], "exit") == 0){
           printf("Bye!\n");
           return -1;
   }
        
        //       if user want to change directory
    if(strcmp(tokens[0], "cd") == 0){
         if(*num_tokens == 2){
              change_directory(tokens[1]);
         }
         return 1;
    }


    child_pid = fork();
    if (child_pid == 0) {
        int e = execvp(tokens[0], tokens);
        if (e == -1) {
            perror("execvp");
            return 0;
        }
    }else if(child_pid < 0){
        perror("fork");
        return 0;
    }else {

        if(!job_type){
            waitpid(child_pid, &status, 0);
        }else {
            Process process = {
                .created_time = clock(),
                .pid = child_pid,
                .status = 0,
            };
            add_process(&p_table, &process);
        }
    }
    return 1;
}

