#include "shell.h"
#include <stdio.h>

typedef struct Command{
    char *name;
    char *input_file;
    char *output_file;
    int argc;
    struct Command *next;

}Command;

Command * add_command(Command *command_list, Command c){
   Command *new_command = malloc(sizeof(Command));
   new_command->name = c.name;
   new_command->input_file = c.input_file;
   new_command->output_file = c.output_file;
   new_command->argc = c.argc;
   new_command->next = c.next;

   if(command_list == NULL){
       command_list = new_command;
       return new_command;
   }

   while(command_list->next != NULL){
       command_list = command_list->next;
   }
   command_list->next = new_command;
   return new_command;
}

int parse(Token *token_list, int *num_tokens){
    int i = 0;
    int stop = 0;
    Command *command_list = NULL;
    Command *tail_command = NULL;
    while(!stop){
        switch(token_list[i].type){
            case COMMAND:
                tail_command = add_command(command_list, (Command){
                    .name = token_list[i].str,
                    .input_file = NULL,
                    .output_file = NULL,
                    .argc = 0,
                    .next = NULL
                });
               break;
            case ARGUMENT:
                tail_command->argc++;
                break;
            case REDIRECTIN:
                if(token_list[i+1].type == ARGUMENT){
                    command_list->input_file = token_list[i+1].str;
                    i++;
                }else{
                    printf("Error in redirection\n");
                    stop = 1;
                }
                break;
            case REDIRECTOUT:
                if(token_list[i+1].type == ARGUMENT){
                    command_list->output_file = token_list[i+1].str;
                    i++;
                }else{
                    printf("Error in redirection\n");
                    stop = 1;
                }
                break;
            case PIPE:
                if(i > 0 && i+1 < *num_tokens && 
                        token_list[i - 1].type == COMMAND ||
                        token_list[i - 1].type == ARGUMENT
                        && token_list[i + 1].type == COMMAND){
                    //skip syntax is correct
                }else{
                    printf("Error in pipe operator\n");
                    stop = 1;
                }
                break;

        }
        i++;
    }
    return !stop;
}
