#include "shell.h"

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
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
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

		if (strcmp(args[0], "exit") == 0)
			handle_exit(args, line, status);
		else if (strcmp(args[0], "env") == 0)
			handle_env();
		else
			execute_command(args, argv, &status);

		free_tokens(args);
	}

	free(line);
	return (status);
}

