#include "shell.h"
#include <sys/stat.h>

/**
 *	check_absolute - Checks if the given command is an absolute or relative path
 *	@command: The command string to check
 *
 *	Return: A duplicated string of the command if it exists, NULL otherwise.
 */

/**
 *	get_path_env - Retrieves the PATH variable from the environment
 *
 *	Return: Pointer to the PATH value if found, or NULL otherwise.
 */

/**
 *	search_in_path - Searches directories listed in PATH for an executable
 *	@path_copy: A duplicated PATH string (will be tokenized)
 *	@command: The command name to search for
 *
 *	Return: malloc'd full path string if found, or NULL otherwise.
 */

/**
 *	find_path - Finds the full path of a command using PATH
 *	@command: The command to search for
 *
 *	Return: Full path string (malloc'd), or NULL if not found
 */
char *find_path(char *command)
{
	char *path_env = NULL, *path_copy, *dir, *full_path;
	int len, i = 0;
	struct stat st;

	if (command == NULL)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		else
			return (NULL);
	}        
	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_env = environ[i] + 5;
			break;
		}
		i++;
	}

	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", dir, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
