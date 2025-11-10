#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/* Parsing and utility functions */
char **parse_line(char *line);
void free_tokens(char **tokens);
char *find_path(char *command);
int _strlen(const char *s);

/* New helper functions for main.c */
char *read_input(void);
int handle_builtins(char **args, char **argv);
int execute_command(char **args, char **argv);

/* New helper functions for main.c */
ssize_t prompt_and_read(char **line, size_t *len);
int execute_command(char **args, char **argv, int *status);

#endif
