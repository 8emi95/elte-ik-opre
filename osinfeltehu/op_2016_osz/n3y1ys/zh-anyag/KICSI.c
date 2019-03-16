#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>
#include <sys/shm.h>
#include <semaphore.h>

#define MEMSIZE 1024
#define SIGERTED SIGRTMIN + 1
#define SIGERTEM SIGRTMIN + 2
#define SIGNEMERTEM SIGRTMIN + 3

struct message
{
    long mtype;
    char mtext[256];
};

int main(int argc, char** argv)
{
    srand(time(NULL));
    key_t key = ftok(argv[0], 1);
    int firstChild, secondChild, parent;
    int msgSnd = msgget(key, 0600 | IPC_CREAT);
    int pipeMsg[2];
    char* sname = "/election";
    
    if (pipe(pipeMsg) == -1)
    {
        perror("Hiba a pipe létrehozásakor.");
        exit(EXIT_FAILURE);
    }
    
    parent = getpid();
    firstChild = fork();
    if (firstChild == 0)
    {
        struct message msg;
        msgrcv(msgSnd, &msg, 256, 1, 0);
        printf("%s \n", msg.mtext);
        sleep(1);
        char buffer[256];
        int random = rand() % 4 + 1;
        sprintf(buffer, "Első gyerek válasza: %d \n", random);
        write(pipeMsg[1], buffer, strlen(buffer) + 1);
    }
    else
    {
        secondChild = fork();
        if (secondChild == 0)
        {
            struct message msg;
            msgrcv(msgSnd, &msg, 256, 2, 0);
            printf("%s \n", msg.mtext);
            sleep(2);
            char buffer[256];
            int random = rand() % 4 + 1;
            sprintf(buffer, "Második gyerek válasza: %d \n", random);
            write(pipeMsg[1], buffer, strlen(buffer) + 1);
        }
        else
        {
            struct message msg = { 1, "Mennyi 1+1?" };
            msgsnd(msgSnd, &msg, strlen(msg.mtext) + 1, 0);
            char buffer[256];
            pause();
            read(pipeMsg[0], buffer, 256);
            printf(buffer);
        }
        
    }
}