#include "shell.h"

/**
 * handle_exit - Handles the exit command
 * @args: Argument vector
 * @line: Input line
 * @status: Exit status
 */
void handle_exit(char **args, char *line, int status)
{
	free_tokens(args);
	free(line);
	exit(status);
}

/**
 * handle_env - Prints the environment variables
 */
void handle_env(void)
{
	int i = 0;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
