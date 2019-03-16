#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

struct msg
{
    long mtype;
    char mtext[1000];
};

int send(int msg, char *msg_text)
{
    int status = 0;
    struct msg message = {1, ""};
    strcat(message.mtext, msg_text);
    status = msgsnd(msg, &message, strlen(message.mtext) + 1, 0);
    if (status < 0)
    {
        perror("Message send error!");
    }
    return 0;
}

int receive(int msg)
{
    struct msg message;
    int status = msgrcv(msg, &message, 1000, 1, 0);

    if (status < 0)
    {
        perror("Message recieve error!");
    }
    else
    {
        printf("%s\n", message.mtext);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    time_t t;
    srand(time(&t));

    key_t key;
    int message, st;
    int pipefd[2];

    key = ftok(argv[0], 1);
    message = msgget(key, 0666 | IPC_CREAT);

    if (message < 0)
    {
        perror("Message get error ");
        return 1;
    }

    if (pipe(pipefd) == -1)
    {
        perror("Pipe open error!");
        return 1;
    }

    pid_t pid1, pid2;
    pid1 = fork();
    int status;
    if (pid1 == 0) // childe1
    {
        printf("Hallgato 1\n");
        sleep(2);
        receive(message);

        close(pipefd[0]);
        int r = rand() % 5 + 1;
        char r_to_s[2];
        sprintf(r_to_s, "%d", r);
        write(pipefd[1], r_to_s, strlen(r_to_s));
        close(pipefd[1]);
    }
    else
    {
        pid2 = fork();
        if (pid2 == 0) // child2
        {
            sleep(3);
            printf("Hallgato 2\n");
            sleep(2);
            receive(message);

            close(pipefd[0]);
            int r = rand() % 5 + 1;
            char r_to_s[2];
            sprintf(r_to_s, "%d", r);
            write(pipefd[1], r_to_s, strlen(r_to_s));
            close(pipefd[1]);
        }
        else
        { // parent
            send(message, "Feltettem egy kerdest");
            waitpid(pid1, &status, 0);

            char elso_valasz[200] = "";
            close(pipefd[1]);
            read(pipefd[0], elso_valasz, sizeof(elso_valasz));
            close(pipefd[0]);
            printf("Valasz1: %s\n", elso_valasz);

            send(message, "Feltettem egy kerdest");
            waitpid(pid2, &status, 0);
            close(pipefd[1]);
            read(pipefd[0], elso_valasz, sizeof(elso_valasz));
            close(pipefd[0]);
            printf("Valasz2: %s\n", elso_valasz);

            printf("Szulo befejezte! (%d)\n", getppid());
        }
    }
}