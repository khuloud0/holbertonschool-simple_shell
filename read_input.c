#include "Shell.h"

/**
 * read_input - Reads a line from stdin
 *
 * Return: Pointer to the line read, or NULL on EOF/error
 */
char *read_input(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);

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
