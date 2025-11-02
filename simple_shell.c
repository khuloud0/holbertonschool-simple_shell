#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            free(line);
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        pid = fork();
        if (pid == 0)
        {
            char *argv[2];
            argv[0] = line;
            argv[1] = NULL;

            if (execve(line, argv, NULL) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            wait(&status);
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
