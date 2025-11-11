#include "shell.h"

/**
 * prompt_and_read - reads a line from stdin and displays the shell prompt.
 * @line: pointer to the input line buffer.
 * @len: pointer to the buffer length.
 *
 * Return: number of characters read, or -1 on failure.
 */
ssize_t prompt_and_read(char **line, size_t *len)
{
	ssize_t read;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);

	read = getline(line, len, stdin);
	if (read == -1 && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);

	return (read);
}

/**
 * process_input - parses and executes user input.
 * @line: input string entered by the user.
 * @argv: argument vector of the shell program.
 * @status: pointer to the variable storing the last command status.
 *
 * Return: nothing.
 */
void process_input(char *line, char **argv, int *status)
{
	char **args = parse_line(line);

	if (!args || !args[0])
	{
		free_tokens(args);
		return;
	}

	if (strcmp(args[0], "exit") == 0)
		handle_exit(args, line, *status);
	else if (strcmp(args[0], "env") == 0)
		handle_env();
	else
		execute_command(args, argv, status);

	free_tokens(args);
}

/**
 * main - entry point for the shell program.
 * @argc: argument count.
 * @argv: argument vector.
 *
 * Return: exit status of the last executed command.
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int status = 0;

	(void)argc;

	while (1)
	{
		read = prompt_and_read(&line, &len);
		if (read == -1)
			break;

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		process_input(line, argv, &status);
	}

	free(line);
	return (status);
}
