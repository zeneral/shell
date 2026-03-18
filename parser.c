#include "shell.h"
#include <stdio.h>
#include <string.h>

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

void display_commands(Command *command_list){
    Command *current_command = command_list;
    while(current_command != NULL){
        printf("Command: ");
        for(int i = 0; i <= current_command->argc; i++)
            printf("%s ", current_command->argv[i]);
        printf("\nInput file: %s\n", current_command->input_file);
        printf("Output file: %s\n", current_command->output_file);
        current_command = current_command->next;
    }
}

int parse(Token *token_list, int *num_tokens, Command **cmd_list){
    int i = 0;
    int stop = 0;
    Command *command_list = NULL;
    Command *tail_command = NULL;
    while(!stop && i <= *num_tokens){
        switch(token_list[i].type){
            case COMMAND:
                tail_command = add_command(&command_list, (Command){
                    .input_file = NULL,
                    .output_file = NULL,
                    .argc = 0,
                    .next = NULL
                });
                tail_command->argv[tail_command->argc] = token_list[i].str;
               break;
            case ARGUMENT:
                tail_command->argc++;
                tail_command->argv[tail_command->argc] = token_list[i].str;
                break;
            case REDIRECTIN:
                if(token_list[i+1].type == ARGUMENT){
                    tail_command->input_file = token_list[i+1].str;
                    i++;
                }else{
                    printf("Error in redirection\n");
                    stop = 1;
                }
                break;
            case REDIRECTOUT:
                if(token_list[i+1].type == ARGUMENT){
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
                }else{
                    printf("Error in pipe operator\n");
                    stop = 1;
                }
                break;

        }
        i++;
    }

    *cmd_list = command_list;
    return !stop;
}
