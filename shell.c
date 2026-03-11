#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
Token * tokenize(char *str, char **tokens, int *num_tokens){
    if(str[0] == '\0'){
        return NULL;
    }
	int k = 0;
    int i = 0;
    Token *token_list = malloc(sizeof(Token)*10);
	char *token;
	char *saveptr;
    token_list[k++].str = &str[i];
    for(; str[i] != '\0' && str[i] != ' ' && str[i] != '\n'; i++);
    for(; str[i] != '\0'; i++){
        if(str[i] == '|'){
            token_list[k].str = &str[i];
            token_list[k].type = PIPE;
        } else if(str[i] == '&'){
            token_list[k].str =  &str[i];
            token_list[k].type = BACKGROUND;
        }else if(str[i] == '>'){
            token_list[k].str = &str[i];
            token_list[k].type = REDIRECT;
        }else if(str[i] == '<'){
            token_list[k].str = &str[i];
            token_list[k].type = REDIRECT;
        }else if(str[i] == ' '){
            // can add spacial character in future
            continue;
        }else if(str[i] == '"'){
            if(str[i + 1] != '\0' && str[i + 1] != '"'){
                token_list[k].str = &str[++i];
                token_list[k].type = ARGUMENT;
            }
            for(; str[i] != '\0' && str[i] != '"'; i++);
        }else if(str[i] == '='){
            token_list[k].str = &str[i];
            token_list[k].type = ASSIGNMENT;
        }else{
            token_list[k].str = &str[i];
            token_list[k].type = ARGUMENT;
            for(; str[i] != '\0' && str[i] != ' ' && str[i] != '\n'; i++);
        }
        k++;
    }

	// token = strtok_r(str, " \n", &saveptr);
	// while (token != NULL) {
		// tokens[i] = token;
		// i++;
		// token = strtok_r(NULL, " \n", &saveptr);
	// }
	// *num_tokens = i;
    // tokens[i++] = NULL;
	// return 0;
    *num_tokens = k;
    return token_list;
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

    if(strcmp(tokens[0], "jobs") == 0){
        display_process_table(&p_table);
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
                .created_time = time(NULL),
                .pid = child_pid,
                .status = 0,
            };
            add_process(&p_table, &process);
        }
    }
    return 1;
}

