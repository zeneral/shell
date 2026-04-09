#ifndef MINISHELL
#define MINISHELL

#include<stdlib.h>
#include<stdio.h>
// job type (background and foreground)
#define BG 0
#define FG 1

enum token_type {
    COMMAND,
    ARGUMENT,
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

typedef struct Command{
    char **argv;
    char *input_file;
    char *output_file;
    int argc;
    size_t size;
    size_t jobtype;
    struct Command *next;

}Command;

typedef struct E_var {
    char *name;
    char *value;
}E_var;

typedef struct E_table {
    E_var *table;
    size_t size;
    size_t count;
}E_table;


extern pid_t child_pid;
extern E_table e_table;

//debug

void display_tokens(Token *token, int token_count);
void display_commands(Command *command_list);
// void display_process_table(ProcessTable *p_table);
void display_env_table();

Token * tokenize(char *str, int *num_tokens);
int run_process(Command *command_list);
int change_directory(char *dir);
int jobtype(char **tokens, int *num_tokens);

//parser 
void display_commands(Command *command_list);
int parse(Token *token_list, int *num_tokens, Command **cmd_list);

//env variable
int set_env_var();

//free memory
void free_command_list(Command **command_list);
void free_env_table();


#endif // !MINISHELL
