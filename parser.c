#include "shell.h"
#include <stdio.h>
#include <string.h>

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

        }else if(str[i] == '<'){
            if(flag == READING_TOKEN) k++;
            token_list[k].str = &str[i];
            token_list[k].type = REDIRECTIN;
            str[i] = '\0';
            flag = TOKEN_DONE;

        }else if(str[i] == '>'){
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
            if(str[i + 1] != '\0' && str[i + 1] != '\n' && str[i + 1] != '"'){
                token_list[k].str = &str[i+1];
                i++;
                if(k > 0 && token_list[k - 1].type == ASSIGNMENT){
                    token_list[k].type = VALUE;
                }else
                    token_list[k].type = ARGUMENT;
            }
            for(; str[i] != '\0' && str[i] != '"' && str[i] != '\n'; i++); // skip string
            str[i] = '\0'; // add null character to end of token
            flag = TOKEN_DONE;
            //coninue if it is last charecter
            if(str[i + 1] == '\0' || str[i + 1] == '\n'){
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

        }else if(str[i] == '\n'){
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

        if(str[i + 1] == '\0'){
                i++;
                continue;
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

Command * add_command(Command **command_list, Command c){
   Command *new_command = malloc(sizeof(Command));
   memcpy(new_command, &c, sizeof(Command));
   // new_command->argv = c.argv;
   // new_command->input_file = c.input_file;
   // new_command->output_file = c.output_file;
   // new_command->argc = c.argc;
   // new_command->next = c.next;

   if(*command_list == NULL){
       *command_list = new_command;
       return new_command;
   }
    
   Command *current_command = *command_list;
   while(current_command->next != NULL){
       current_command = current_command->next;
   }
   current_command->next = new_command;
   return new_command;
}



int parse(Token *token_list, int *num_tokens, Command **cmd_list){
    int i = 0, v = 1; // i to count token v to count env variable
    int stop = 0;
    Command *command_list = NULL;
    Command *tail_command = NULL;

    e_table.table = (E_var *)malloc(sizeof(E_var) * 5); // initalize with size 5
    e_table.size = 5;
    e_table.count = 0; 

    while(!stop && i <= *num_tokens){
        switch(token_list[i].type){
            case COMMAND:
                // putting null at the end of argv to mark end of command
                if(tail_command != NULL){
                    tail_command->argv[tail_command->argc + 1] = NULL;
                }

                tail_command = add_command(&command_list, (Command){
                    .size = 5,
                    .argv = NULL, 
                    .input_file = NULL,
                    .output_file = NULL,
                    .argc = 0,
                    .jobtype = FG,
                    .next = NULL
                });
                tail_command->argv = malloc(sizeof(char *) * tail_command->size);
                tail_command->argv[tail_command->argc] = token_list[i].str;
               break;
            case ARGUMENT:
                tail_command->argc++;
                if(tail_command->argc > tail_command->size){
                    tail_command->size *= 2;
                    char **temp = realloc(tail_command->argv, sizeof(char *) * tail_command->size);
                    if(temp == NULL){
                        perror("realloc");
                        stop = 1;
                        break;
                    }else{
                        tail_command->argv = temp;
                    }
                }
                tail_command->argv[tail_command->argc] = token_list[i].str;
                break;
            case REDIRECTIN:
                if(i > 0 && i+1 <= *num_tokens && token_list[i+1].type == ARGUMENT
                    && (token_list[i-1].type == ARGUMENT || 
                    token_list[i - 1].type == COMMAND)){
                        tail_command->input_file = token_list[i+1].str;
                        i++;
                }else{
                    printf("Error in redirection\n");
                    stop = 1;
                }
                break;
            case REDIRECTOUT:
                if(i > 0 && i+1 <= *num_tokens && token_list[i+1].type == ARGUMENT
                    && (token_list[i-1].type == ARGUMENT || 
                    token_list[i - 1].type == COMMAND)){
                        tail_command->output_file = token_list[i+1].str;
                        i++;
                }else{
                    printf("Error in redirection\n");
                    stop = 1;
                }
                break;
            case PIPE:
                if(i > 0 && i+1 <= *num_tokens && 
                        token_list[i - 1].type == COMMAND ||
                        token_list[i - 1].type == ARGUMENT
                        && token_list[i + 1].type == COMMAND){

                    //grammer = command argument | comand argument
                    //skip syntax is correct
                tail_command->argv[tail_command->argc + 1] = NULL;
                }else if(i > 0 && i + 1 <= *num_tokens && 
                    token_list[i - 1].type == VALUE && 
                    token_list[i + 1].type == VARIABLE
                    || token_list[i + 1]. type == COMMAND){
                    // grammer = value = variable | command
                    // value = variable | value = variable

                }else{
                    printf("Error in pipe operator\n");
                    stop = 1;
                }
                break;
            case BACKGROUND:
                if(i > 0 && (i <= *num_tokens || token_list[i+1].type == PIPE)
                    && (token_list[i-1].type == ARGUMENT || 
                    token_list[i - 1].type == COMMAND)){
                    // grammer = command argument & | comand argument &
                    tail_command->jobtype = BG;
                    i++;
                }else{
                    printf("Error in background operator\n");
                    stop = 1;
                }
                break;

            case ASSIGNMENT:
                if(i > 0 && (i < *num_tokens) || token_list[i + 1].type == VALUE){
                   if(v == e_table.size){
                        e_table.size += e_table.size;
                        E_var *temp = realloc(e_table.table, sizeof(E_table) * e_table.size);
                        if(temp != NULL){
                            e_table.table = temp;
                        }else{
                            perror("realloc");
                            stop = 1;
                        }
                    }
                    e_table.table[v].name = token_list[i - 1].str;
                    e_table.table[v].value = token_list[i + 1].str;
                    e_table.count = v;                        
                    v++;
                }
                break;
        }
        i++;
    }

    if(tail_command != NULL)
        tail_command->argv[tail_command->argc + 1] = NULL; // add null at the end of argv
    *cmd_list = command_list;
    return !stop;
}



