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

sem_t* makeSemaphore(char* name, int val)
{
    sem_t *semid = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, val);
	if (semid == SEM_FAILED)
		perror("sem_open");
    
    return semid;
}

void deleteSemaphore(char* name)
{
      sem_unlink(name);
}

static void handler(int signo)
{
    // printf("%d jelzés küldése megtörtént! \n", signo);
    if (signo == SIGERTEM)
            printf("A gyerek érti. \n");
    if (signo == SIGNEMERTEM)
            printf("A gyerek nem érti. \n");
            
    signal(signo, handler);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    signal(SIGUSR1, handler);
    signal(SIGERTED, handler);
    signal(SIGERTEM, handler);
    signal(SIGNEMERTEM, handler);
    key_t key = ftok(argv[0], 1);
    int firstChild, secondChild, parent;
    int msgSnd = msgget(key, 0600 | IPC_CREAT);
    int pipeMsg[2];
    char* sname = "/election";
    int sh_mem_id = shmget(key, MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    char* s = shmat(sh_mem_id, NULL, 0);
    sem_t* semid = makeSemaphore(sname, 0);
    
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
        random = rand();
        kill(parent, SIGUSR1);
        pause();
        if (random % 2)
        {
            strcpy(s, "Egyes gyerek nem érti. \n");
            random = rand();
            if (random % 2)
                kill(parent, SIGERTEM);
            else
                kill(parent, SIGNEMERTEM);
        }
        sem_post(semid);
        shmdt(s);
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
            random = rand();
            kill(parent, SIGUSR1);
            pause();
            if (random % 2)
            {
                strcpy(s, "Második gyerek nem érti. \n");
                random = rand();
                if (random % 2)
                    kill(parent, SIGERTEM);
                else
                    kill(parent, SIGNEMERTEM);
            }
            sem_post(semid);
            shmdt(s);
        }
        else
        {
            struct message msg = { 1, "Mennyi 1+1?" };
            msgsnd(msgSnd, &msg, strlen(msg.mtext) + 1, 0);
            char buffer[256];
            pause();
            read(pipeMsg[0], buffer, 256);
            printf(buffer);
            msg.mtype = 2;
            strcpy(msg.mtext, "Mennyi 1+2?");
            msgsnd(msgSnd, &msg, strlen(msg.mtext) + 1, 0);
            pause();
            read(pipeMsg[0], buffer, 256);
            printf(buffer);
            kill(firstChild, SIGERTED);
            kill(secondChild, SIGERTED);
            sem_wait(semid);
            printf(s);
            sem_post(semid);
            shmdt(s);
            wait(NULL);
            deleteSemaphore(sname);
            shmctl(sh_mem_id, IPC_RMID, NULL);
        }
        
    }
}