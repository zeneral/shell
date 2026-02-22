#ifndef MINISHELL
#define MINISHELL


int tokenize(char *str, char **tokens, int *num_tokens);
int run_process(char **tokens, int *num_tokens);

#endif // !MINISHELL
