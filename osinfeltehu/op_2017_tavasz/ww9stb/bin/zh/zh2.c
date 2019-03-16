#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


void taxiArrived(int sig) {
    printf("Taxi megérkezett a címre\n");
}

int main(int argc, char *argv[]) {
    int pipefd[2]; // 0 - read 1 - write
    pid_t pid, pid2;
    int status, status2;
    char sz[100];
    char sz2[100];


    if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        close(pipefd[0]); // close read

        write(pipefd[1], argv[1], sizeof(argv[1])); // write
        printf("Gyerek elkuldte a címet\n");

        close(pipefd[1]); // close write

        signal(SIGRTMIN, taxiArrived);
        pause();
    }
    else {
        // szulo process
        printf("Szulo indul!\n");

        // close(pipefd[1]); // close write

        read(pipefd[0], &sz, sizeof(sz)); // read

        printf("Szulo megkapta a lakcímet:\n\n\t %s\n\n", sz);

        if ((pid2 = fork()) == 0) {
            // taxis child
            read(pipefd[0], &sz2, sizeof(sz2)); // read

            printf("Taxi megkapta a címet és telefonszámot: \n\n \t %s \n\n", sz2);
            printf("Taxi elindult az utasért.\n");

            char address[100];
            int child_pid;

            sscanf(sz2, "%s - %d", address, &child_pid);

            sleep(2);
            kill(child_pid, SIGRTMIN);
        } else {
            char send[200];

            sprintf(send, "%s - %d", sz, pid);
            write(pipefd[1], send, sizeof(send));

            waitpid(pid, &status, 0);
            waitpid(pid2, &status2, 0);
            printf("Szulo befejezte!\n");
        }
    }

    return 0;
}


