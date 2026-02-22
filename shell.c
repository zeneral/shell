#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

int run_process(char **tokens, int *num_tokens){
    int pid,status;
    pid = fork();
    if (pid == 0) {
        int e = execvp(tokens[0], tokens);
        if (e == -1) {
            perror("execvp");
            exit(1);
        }
    }else if(pid < 0){
        perror("fork");
        exit(1);
    }else {
        wait(&status);
    }
    return pid;
}

