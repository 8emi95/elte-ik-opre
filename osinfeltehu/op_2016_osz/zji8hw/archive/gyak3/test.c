#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main()
{
    int pipefd[2];
    if (pipe(pipefd) < 0)
    {
        perror("jajj");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("jajj");
        exit(1);
    }

    if (pid)
    {
        wait(NULL);

        char r[50];
        read(pipefd[0], r, sizeof(r));
        printf("Parent kapja: %s\n", r);

        close(pipefd[0]);
        close(pipefd[1]);
    }
    else
    {
        char *m = "Macska";
        write(pipefd[1], m, strlen(m) + 1);
        printf("Child küldi: %s\n", m);

        close(pipefd[0]);
        close(pipefd[1]);
    }

    return 0;
}