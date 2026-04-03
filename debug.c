#include "shell.h"
#include<stdio.h>
#include<time.h>


void display_tokens(Token token, int token_count){
    for(int i = 0; i < token_count; i++){
        for(int j = 0; token.str[j] != ' ' && token.str[j] != '\0'; j++){
            printf("%c", token.str[j]);
        }    
    }
}

void display_commands(Command *command_list){
    Command *current_command = command_list;
    while(current_command != NULL){
        printf("Command: ");
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

void display_process_table(ProcessTable *p_table){
    for(int i = 0; i < p_table->process_count; i++){
        printf("PID: %d\t", p_table->table[i].pid);
        printf("Status: %d\t", p_table->table[i].status);
        printf("Created Time: %s\t", ctime(&p_table->table[i].created_time));
        printf("\n");
    }
    return;
}
