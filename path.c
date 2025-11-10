#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 *	check_absolute - Check if command is absolute or relative path
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
 *	Returns pointer to PATH value inside environ (NOT allocated),
 *	or NULL if PATH is not present.
 *	Return: pointer to PATH string inside environ or NULL if not found
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
 *	Returns malloc'd full path if found (caller must free), or NULL.
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
 *	Return: Full path string (malloc'd), or NULL if not found.
 *	If command is absolute/relative and exists, returns strdup'd command.
 *	Caller is responsible for freeing returned pointer.
 */
char *find_path(char *command)
{
	char *path_env = NULL, *path_copy = NULL, *result = NULL;

	if (command == NULL)
		return (NULL);

	/* If absolute or relative path, use it directly if executable */
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

