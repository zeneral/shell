#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"



int get_input(char *input, size_t input_size){
    int i = 0;
    while(1){
        if(i == input_size){
            return -1;
        }
        int c = getchar();
        if(c == '\n'){
            break;
        }
        if(c == 18){
             
        }
        input[i++] = c;
    }
    input[i] = '\0';
    return 0;
}

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

int save_history(char history[10][10], char *input, int *history_index){
    *history_index = *history_index % 10;
    int i;
    for(i = 0; input[i] != '\0'; i++){
        history[*history_index][i] = input[i];
    }
    history[*history_index][i] = '\0';
    (*history_index)++;
    return 0;
}

int run_process(char **tokens, int *num_tokens){
    int pid,status;
    pid = fork();
    if (pid == 0) {
        int e = execvp(tokens[0], tokens);
        if (e == -1) {
            perror("execvp");
            return -2;
        }
    }else if(pid < 0){
        perror("fork");
        return -1;
    }else {
        wait(&status);
    }
    return pid;
}

