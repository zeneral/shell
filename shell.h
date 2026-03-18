#ifndef MINISHELL
#define MINISHELL

#include <stdlib.h>

enum token_type {
    COMMAND,
    ARGUMENT,
    AND,
    PIPE,
    REDIRECTIN,
    REDIRECTOUT,
    BACKGROUND,
    ASSIGNMENT,
    VARIABLE,
    VALUE
};

typedef struct {
    char *str;
    enum token_type type;
} Token;

typedef struct {
    int pid;
    int status;
    char *dir;
    time_t created_time;
}Process;

typedef struct {
    int process_count;
    int table_size;
    Process *table;
}ProcessTable;

typedef struct Command{
    char *argv[10];
    char *input_file;
    char *output_file;
    int argc;
    struct Command *next;

}Command;


extern pid_t child_pid;
extern ProcessTable p_table;


Token * tokenize(char *str, int *num_tokens);
int run_process(char **tokens, int *num_tokens, int job_type);
int change_directory(char *dir);
int jobtype(char **tokens, int *num_tokens);
void monitor_process(ProcessTable *p_table);

//parser 
void display_commands(Command *command_list);
int parse(Token *token_list, int *num_tokens, Command **cmd_list);

//processes
void initialize_process_table(ProcessTable *p_table);
void add_process(ProcessTable *p_table, Process *process);
Process *get_process(ProcessTable *p_table, int pid);
int delete_process(ProcessTable *p_table, int pid);
void destroy_process_table(ProcessTable *p_table);
void display_process_table(ProcessTable *p_table);
#endif // !MINISHELL
