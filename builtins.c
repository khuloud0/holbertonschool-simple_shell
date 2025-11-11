#include "shell.h"

/**
 * handle_exit - Exits the shell with a given status
 * @args: Array of arguments passed to the command
 * @line: Input line buffer to be freed before exit
 * @status: The last executed command's status
 *
 * Description: Frees memory and exits with the given status code.
 * Return: Nothing (void)
 */

/**
 *	handle_builtins - Handle shell built-in commands
 *	@args: NULL-terminated array of arguments (args[0] is the command)
 *	@status: Pointer to shell status variable to update if needed
 *
 *	Checks whether args[0] is a builtin (like "exit" or "env") and runs
 *	the appropriate action. Returns 1 if the command was a builtin and
 *	handled, or 0 if not (caller should attempt external execution).
 *	Return: 1 if builtin handled, 0 otherwise
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
