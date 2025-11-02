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
 * @line: pointer to string
 * Return: pointer to trimmed string
 */
char *trim_line(char *line)
{
	char *start, *end;

	if (!line)
		return (NULL);

	start = line;
	while (*start == ' ' || *start == '\t')
		start++;

	end = line + strlen(line) - 1;
	while (end > start && (*end == ' ' || *end == '\t'))
	{
		*end = '\0';
		end--;
	}

	return (start);
}

/**
 * read_line - reads a line from stdin
 * Return: pointer to line (must be freed by caller)
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
		return (NULL);
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * execute_command - forks and executes a command
 * @line: command to execute
 */
void execute_command(char *line)
{
	pid_t child_pid;
	char *argv[] = {line, NULL};

	if (!line || *line == '\0')
		return;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return;
	}

	if (child_pid == 0)
	{
		if (execve(line, argv, NULL) == -1)
		{
			perror("simple_shell");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(NULL);
}

/**
 * main - Simple Shell 0.1
 * Return: 0
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

	return (0);
}
