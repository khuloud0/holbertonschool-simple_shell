#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 *	check_absolute - Check if command is an absolute or relative path
 *	@command: Command string supplied by user
 *
 *	If command starts with '/' or '.' and exists, returns a strdup'd path.
 *	Caller must free returned string. Returns NULL otherwise.
 *	Return: strdup'd path when command exists, or NULL on failure
 */

static char *check_absolute(char *command)
{
	struct stat st;

	if (command == NULL)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}
	return (NULL);
}

/**
 *	get_path_env - Retrieve PATH value from environ
 *
 *	Searches the process environment for the "PATH=" entry and returns
 *	a pointer to the PATH value within environ (not allocated).
 *	Return: pointer to PATH string inside environ, or NULL if not found
 */

static char *get_path_env(void)
{
	int i = 0;

	if (!environ)
		return (NULL);

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 *	search_in_path - Search PATH directories for executable command
 *	@path_copy: A modifiable strdup'd copy of PATH (will be tokenized)
 *	@command: Command name to search for
 *
 *	Iterates directories in path_copy and builds candidate paths for the
 *	command. Returns a malloc'd full path if an executable is found.
 *	Caller must free the returned string.
 *	Return: malloc'd full path string if executable found, or NULL otherwise
 */

static char *search_in_path(char *path_copy, char *command)
{
	char *dir, *full_path;
	int len;

	if (!path_copy || !command)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + 1 + strlen(command) + 1; /* dir + '/' + cmd + '\0' */
		full_path = malloc(len);
		if (!full_path)
			return (NULL);

		sprintf(full_path, "%s/%s", dir, command);

		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		dir = strtok(NULL, ":");
	}
	return (NULL);
}

/**
 *	find_path - Finds the full path of a command using PATH
 *	@command: The command to search for
 *
 *	Checks whether command is an absolute/relative path and uses it if
 *	executable. Otherwise searches PATH entries for an executable with
 *	the given name and returns a malloc'd full path.
 *	Caller is responsible for freeing the returned pointer.
 *	Return: malloc'd full path or strdup'd command when found, NULL otherwise
 */

char *find_path(char *command)
{
	char *path_env = NULL, *path_copy = NULL, *result = NULL;

	if (command == NULL)
		return (NULL);

	result = check_absolute(command);
	if (result != NULL)
		return (result);

	path_env = get_path_env();
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	result = search_in_path(path_copy, command);

	free(path_copy);
	return (result);
}

