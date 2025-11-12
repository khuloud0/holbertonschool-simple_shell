#include "shell.h"
#include <sys/stat.h>

/**
 * get_path_env - Retrieves the PATH environment variable
 *
 * Return: Pointer to PATH value, or NULL if not found
 */
char *get_path_env(void)
{
	int i = 0;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * build_full_path - Creates a full path string from directory and command
 * @dir: The directory from PATH
 * @command: The command to append
 *
 * Return: Full path (malloc'd) or NULL on failure
 */
char *build_full_path(char *dir, char *command)
{
	char *full_path;
	int len = strlen(dir) + strlen(command) + 2;

	full_path = malloc(len);
	if (!full_path)
		return (NULL);

	sprintf(full_path, "%s/%s", dir, command);
	return (full_path);
}

/**
 * find_path - Finds the full path of a command using PATH
 * @command: The command to search for
 *
 * Return: Full path string (malloc'd), or NULL if not found
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	struct stat st;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
		return (stat(command, &st) == 0 ? strdup(command) : NULL);

	path_env = get_path_env();
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = build_full_path(dir, command);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

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
