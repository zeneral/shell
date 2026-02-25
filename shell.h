#ifndef MINISHELL
#define MINISHELL

#include <stdlib.h>

extern pid_t child_pid;

int tokenize(char *str, char **tokens, int *num_tokens);
int run_process(char **tokens, int *num_tokens);
int change_directory(char *dir);
int save_history(char history[10][10], char *input, int *history_index);
#endif // !MINISHELL
