#include <string.h>

int tokenize(char *str, char **tokens, int *num_tokens){
	int i = 0;
	char *token;
	char *saveptr;

	token = strtok_r(str, " \n", &saveptr);
	while (token != NULL) {
		tokens[i] = token;
		i++;
		token = strtok_r(NULL, " ", &saveptr);
	}
	*num_tokens = i;
	return 0;
}
