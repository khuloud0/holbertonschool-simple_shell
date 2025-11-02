#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;
        printf("%s", line);
    }
    free(line);
    return (0);
}
