#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <signal.h>

#define BUFFER_SIZE 128;

struct message {
    char address[256];
    int phoneNumber;
};

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Argumentumkent szukseges a cim");
        return 0;
    }

    int i, j,status;

    char *kulcs = argv[0];
    char* address = argv[1];


    int pipe1[2];

    struct message message;


    int mqid;
    key_t key = ftok(kulcs, 1);
    mqid = msgget(key, 0644 | IPC_CREAT);

    pid_t pid1;

    if (pipe(pipe1) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }


    if (pid1 != 0) {

        pid_t pid2;
        int pipe2[2];

        char msg[16];

        if (pipe(pipe2) == -1) {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }

        pid2 = fork();
        if (pid2 == -1) {
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }

        if (pid2 != 0) {

            sleep(2);
            close(pipe1[1]);
            read(pipe1[0], &message, sizeof(message));
            close(pipe1[0]);
            printf("Kozpont: A kapott cim: %s\n", message.address);
            message.phoneNumber = pid1;
            close(pipe2[0]);
            write(pipe2[1], &message, sizeof(message));
            close(pipe2[1]);
            fflush(NULL);
            sleep(2);
            waitpid(pid1,&status,0);
            waitpid(pid2,&status,0);

        } else {
            sleep(3);
            read(pipe2[0], &message, sizeof(message));
            close(pipe2[0]);
            printf("Taxi: A cim: %s\nA telefonszam: %d\n", message.address, message.phoneNumber);
            strcpy((char *) &msg, "elindultam");
            write(pipe2[1], &msg, sizeof(msg));
            close(pipe2[1]);

        }

    } else {
        sleep(1);

        strcpy((char*) &(message.address), address);

        close(pipe1[0]);
        write(pipe1[1], &message, sizeof(message));
        close(pipe1[1]);
        fflush(NULL);
    }

    return 0;
}
