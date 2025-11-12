#include "shell.h"

/**
 * read_command - Reads input from stdin
 * @line: Pointer to input buffer
 * @len: Pointer to buffer length
 *
 * Return: Number of characters read, or -1 on failure
 */
ssize_t read_command(char **line, size_t *len)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);

	return (getline(line, len, stdin));
}

/**
 * handle_builtin - Handles built-in commands like exit and env
 * @args: Tokenized command arguments
 * @line: Input line buffer
 * @status: Pointer to status variable
 *
 * Return: 1 if command was built-in and handled, 0 otherwise
 */
int handle_builtin(char **args, char *line, int *status)
{
	int i;

	if (strcmp(args[0], "exit") == 0)
	{
		free_tokens(args);
		free(line);
		exit(*status);
	}

	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		free_tokens(args);
		return (1);
	}
	return (0);
}

/**
 * execute_command - Executes external commands
 * @args: Tokenized command arguments
 * @argv: Argument vector from main
 * @status: Pointer to status variable
 */
void execute_command(char **args, char **argv, int *status)
{
	char *full_cmd = find_path(args[0]);
	pid_t child_pid;

	if (!full_cmd)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", argv[0], args[0]);
		free_tokens(args);
		*status = 127;
		return;
	}

	child_pid = fork();
	if (child_pid == 0)
	{
		if (execve(full_cmd, args, environ) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(status);
		if (WIFEXITED(*status))
			*status = WEXITSTATUS(*status);
		else
			*status = 2;
	}

	free(full_cmd);
	free_tokens(args);
}

/**
 * main - Entry point for the shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status of the last command
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int status = 0;

	(void)argc;

	while (1)
	{
		read = read_command(&line, &len);
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		args = parse_line(line);
		if (!args || !args[0])
		{
			free_tokens(args);
			continue;
		}

		if (!handle_builtin(args, line, &status))
			execute_command(args, argv, &status);
	}

	free(line);
	return (status);
}

