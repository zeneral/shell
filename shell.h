#ifndef MINISHELL
#define MINISHELL


int tokenize(char *str, char **tokens, int *num_tokens);
int run_process(char **tokens, int *num_tokens);
int change_directory(char *dir);
int save_history(char ** history, char *input, int *history_index);
#endif // !MINISHELL
