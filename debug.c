#include "shell.h"
#include<stdio.h>
#include<time.h>


void display_tokens(Token *token, int token_count){
    for(int i = 0; i <= token_count; i++){
        printf("token: ");
        switch(token[i].type){
            case ARGUMENT:
            case COMMAND:
            case VALUE:
            case VARIABLE:
                printf("%s", token[i].str);
                break;
            case PIPE:
                printf("|");
                break;
            case BACKGROUND:
                printf("&");
                break;
            case REDIRECTOUT:
                printf(">");
                break;
            case REDIRECTIN:
                printf("<");
                break;
            case ASSIGNMENT:
                printf("=");
                break;
        }
        printf("\ttype: %d\n", token[i].type);
    }
}

void display_commands(Command *command_list){
    Command *current_command = command_list;
    while(current_command != NULL){
        printf("\n\nCommand: ");
        for(int i = 0; i <= current_command->argc; i++)
            printf("%s ", current_command->argv[i]);
        printf("\nInput file: %s\n", current_command->input_file);
        printf("Output file: %s\n", current_command->output_file);
        if(current_command->jobtype == BG)
            printf("Job type: Background\n");
        else
            printf("Job type: Foreground\n");
        printf("--------------------\n");
        current_command = current_command->next;
    }
}

void display_env_table(){
    if(e_table.count < 1)
        return;
    printf("\nenv table\n");
    for(int i=1; i <= e_table.count; i++){
        printf("Name: %s => value: %s\n", e_table.table[i].name, e_table.table[i].value);
    }
}


