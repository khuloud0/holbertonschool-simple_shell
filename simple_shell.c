#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		printf("$ ");
}

/**
 * trim_line - removes leading and trailing spaces
 */
char *trim_line(char *line)
{
	char *start, *end;

	if (!line)
		return NULL;

	start = line;
	while (*start == ' ' || *start == '\t')
		start++;

	end = line + strlen(line) - 1;
	while (end > start && (*end == ' ' || *end == '\t'))
	{
		*end = '\0';
		end--;
	}

	return start;
}

/**
 * read_line - reads a line from stdin
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return NULL;
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return line;
}

/**
 * execute_command - forks and executes a command
 */
void execute_command(char *line)
{
	pid_t pid;
	char **argv;

	if (!line || *line == '\0')
		return;

	argv = malloc(2 * sizeof(char *));
	if (!argv)
	{
		perror("malloc");
		return;
	}

	argv[0] = line;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(argv);
		return;
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, NULL) == -1)
		{
			perror("simple_shell");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(NULL);

	free(argv);
}

/**
 * main - simple shell
 */
int main(void)
{
	char *line, *trimmed;

	while (1)
	{
		print_prompt();
		line = read_line();
		if (!line)
		{
			printf("\n");
			break;
		}

		trimmed = trim_line(line);
		execute_command(trimmed);
		free(line);
	}

	return 0;
}
