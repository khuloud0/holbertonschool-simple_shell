#include "shell.h"

/**
 * execute_command - Executes a command
 * @args: Argument vector
 * @argv: Argument list from main
 * @status: Pointer to status variable
 */
void execute_command(char **args, char **argv, int *status)
{
	char *full_cmd = find_path(args[0]);
	pid_t pid;

	if (full_cmd == NULL)
	{
		dprintf(STDERR_FILENO, "%s: 1: %s: not found\n", argv[0], args[0]);
		*status = 127;
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_cmd);
		*status = 2;
		return;
	}

	if (pid == 0)
	{
		if (execve(full_cmd, args, environ) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	wait(status);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else
		*status = 2;

	free(full_cmd);
}
