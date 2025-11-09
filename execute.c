#include "Shell.h"

/**
 * execute_command - Forks and executes a command
 * @args: Command arguments
 * @argv: Program name (for errors)
 *
 * Return: Exit status of the command
 */
int execute_command(char **args, char **argv)
{
	pid_t pid;
	int status;
	char *full_cmd;

	full_cmd = find_path(args[0]);
	if (!full_cmd)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", argv[0], args[0]);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(full_cmd, args, environ) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(&status);

	free(full_cmd);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (2);
}
