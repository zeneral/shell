#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
Token * tokenize(char *str, int *num_tokens){

    enum state{
        TOKEN_DONE,
        READING_TOKEN,
    };

    if(str[0] == '\0'){
        return NULL;
    }
	int k = 0;
    int i = 0;
    enum state flag = TOKEN_DONE;
    int list_size = 10;
    Token *token_list = malloc(sizeof(Token)*list_size);

    for(;str[i] == ' ' || str[i] == '\t'; i++); // skip white space
                                                
    while(str[i] != '\0'){
        if(str[i] == '|'){
            if(flag == READING_TOKEN) k++;
            token_list[k].str = &str[i];
            token_list[k].type = PIPE;
            str[i] = '\0';
            flag = TOKEN_DONE;

        } else if(str[i] == '&'){
            if(flag == READING_TOKEN) k++;
            token_list[k].str =  &str[i];
            token_list[k].type = BACKGROUND;
            str[i] = '\0';
            flag = TOKEN_DONE;

        }else if(str[i] == '>'){
            if(flag == READING_TOKEN) k++;
            token_list[k].str = &str[i];
            token_list[k].type = REDIRECTIN;
            str[i] = '\0';
            flag = TOKEN_DONE;

        }else if(str[i] == '<'){
            if(flag == READING_TOKEN) k++;
            token_list[k].str = &str[i];
            token_list[k].type = REDIRECTOUT;
            str[i] = '\0';
            flag = TOKEN_DONE;

        }else if(str[i] == ' '){
            str[i] = '\0'; // add null character to end of token
            if(flag == READING_TOKEN){
                // if it was token reading mode then skip white 
                // space and change flag to token done
                // idea is  when it gets space first time after reading
                // a token it will be in token done mode
                k++;    
                flag = TOKEN_DONE;
            }
            //if it was already in token done mode that means it alredy done
            //with reading token so just skip white space works if white space
            //is repeated
            i++;
            continue;

        }else if(str[i] == '"'){
            if(flag == READING_TOKEN) k++;
            str[i] = '\0';
            if(str[i + 1] != '\0' && str[i + 1] != '"'){
                token_list[k].str = &str[++i];
                if(k > 0 && token_list[k - 1].type == ASSIGNMENT){
                    token_list[k].type = VALUE;
                }else
                    token_list[k].type = ARGUMENT;
            }
            for(; str[i] != '\0' && str[i] != '"'; i++); // skip string
            str[i] = '\0'; // add null character to end of token
            flag = TOKEN_DONE;
            //coninue if it is last charecter
            if(str[i + 1] == '\0'){
                i++;
                continue;
            }
            //procced if it is not last charcter

        }else if(str[i] == '='){
            if(flag == READING_TOKEN) k++;
            token_list[k].str = &str[i];
            token_list[k].type = ASSIGNMENT;
            if(k > 0 && token_list[k - 1].type == COMMAND){
                token_list[k - 1].type = VARIABLE;
            }
            str[i] = '\0';
            flag = TOKEN_DONE;

        }else{
            if(flag == TOKEN_DONE){
                token_list[k].str = &str[i];
                if(k == 0 || token_list[k - 1].type == PIPE){
                    token_list[k].type = COMMAND;
                }else if(token_list[k - 1].type == ASSIGNMENT)
                    token_list[k].type = VALUE;
                else
                token_list[k].type = ARGUMENT;
                flag = READING_TOKEN;
            }
        }

        if(flag == TOKEN_DONE){
            k++;
        }
        i++;

        if(k == list_size){
            list_size += 10;
            token_list = realloc(token_list, sizeof(Token) * list_size);
        } 
    }

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

