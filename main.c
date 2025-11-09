#include "Shell.h"

/**
 * print_prompt - Prints the shell prompt if input is from terminal
 */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * handle_input - Reads a line of input from stdin and removes trailing newline
 *
 * Return: Pointer to the input line, or NULL on EOF/error
 */
char *handle_input(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	read = getline(&line, &len, stdin);
	if (read == -1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		return (NULL);
	}

	if (line[read - 1] == '\n')
		line[read - 1] = '\0';

	return (line);
}

/**
 * handle_builtins - Executes built-in commands like exit and env
 * @args: Array of arguments
 * @argv: Array of program arguments (argv[0] is program name)
 *
 * Return: 0 if a builtin was executed, -1 otherwise
 */
int handle_builtins(char **args, char **argv)
{
	int i;

	if (strcmp(args[0], "exit") == 0)
		exit(0);

	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (0);
	}

	return (-1);
}

/**
 * main - Entry point for the shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status of last executed command
 */
int main(int argc, char **argv)
{
	char *line, **args;
	int status = 0;

	(void)argc;

	while (1)
	{
		print_prompt();
		line = handle_input();
		if (!line)
			break;

		args = parse_line(line);
		if (!args || !args[0])
		{
			free_tokens(args);
			free(line);
			continue;
		}

		if (handle_builtins(args, argv) == 0)
		{
			free_tokens(args);
			free(line);
			continue;
		}

		status = execute_command(args, argv);

		free_tokens(args);
		free(line);
	}

	return (status);
}
