#include "shell.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static char *check_absolute(char *command)
{
	struct stat st;

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		else
			return (NULL);
	}
	return (NULL);
}

static char *get_path_env(void)
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

static char *search_in_path(char *command, char *path_env)
{
	char *path_copy, *dir, *full_path;
	int len;

	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
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

char *find_path(char *command)
{
	char *path_env, *result;

	if (!command)
		return (NULL);

	result = check_absolute(command);
	if (result)
		return (result);

	path_env = get_path_env();
	result = search_in_path(command, path_env);

	return (result);
}
