#ifndef MINISHELL
#define MINISHELL

#include <stdlib.h>



typedef struct {
    int pid;
    int status;
    char *dir;
    clock_t created_time;
}Process;

typedef struct {
    int process_count;
    int table_size;
    Process *table;
}ProcessTable;

extern pid_t child_pid;
extern ProcessTable p_table;

int tokenize(char *str, char **tokens, int *num_tokens);
int run_process(char **tokens, int *num_tokens, int job_type);
int change_directory(char *dir);
int jobtype(char **tokens, int *num_tokens);

//processes

void initialize_process_table(ProcessTable *p_table);
void add_process(ProcessTable *p_table, Process *process);
Process *get_process(ProcessTable *p_table, int pid);
int delete_process(ProcessTable *p_table, int pid);

#endif // !MINISHELL
