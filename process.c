#include "shell.h"
#include <stdio.h>
#include <time.h>

void initialize_process_table(ProcessTable *p_table){
    p_table->process_count = 0;
    p_table->table_size = 10;
    p_table->table = malloc(sizeof(Process)*p_table->table_size);
}

void add_process(ProcessTable *p_table, Process *process){
    if(p_table->process_count == p_table->table_size){
        p_table->table_size *= 2;
        p_table->table = realloc(p_table->table, sizeof(Process)*p_table->table_size);
    }
    p_table->table[p_table->process_count] = *process;
    p_table->process_count++;
}

Process *get_process(ProcessTable *p_table, int pid){
    for(int i = 0; i < p_table->process_count; i++){
        if(p_table->table[i].pid == pid){
            return &p_table->table[i];
        }
    }
    return NULL;
}

void destroy_process_table(ProcessTable *p_table){
    free(p_table->table);
    p_table->process_count = 0;
    p_table->table_size = 0;
}

int delete_process(ProcessTable *p_table, int pid){
    Process *process = get_process(p_table, pid);
    if(process == NULL){
        return 0;
    }
    for(int i = 0; i < p_table->process_count; i++){
        if(p_table->table[i].pid == pid){
            p_table->table[i] = p_table->table[p_table->process_count - 1];
            p_table->process_count--;
        }
    }
    return 1;
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
