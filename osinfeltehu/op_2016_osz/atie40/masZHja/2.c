#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

struct message { 
     long mtype;
     char mtext[1024]; 
};

int msgqueue;

void handler(int x) { x = x; }

void child_fn();

int main(int argc, char* argv[])
{
    pid_t child;
    key_t key;
    struct sigaction sigact;
    int hanyadik;
    struct message msg;
    int pot_kell, mentesito_db;

    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGUSR1,&sigact,NULL);

    if (argc < 2)
    {
        printf("Nincs megadva a parancssori parameter (osszes szerelvenyszam)");
        exit(1);
    }

    key = ftok(argv[0], 1);
    msgqueue = msgget(key, 0600 | IPC_CREAT);
    if (msgqueue < 0)
    {
        perror("msgget");
        exit(1);
    }

    srand(time(NULL));
    hanyadik = 0;
    pot_kell = 0;
    mentesito_db = 0;

    while (hanyadik < atoi(argv[1]))
    {
        child = fork();
        if (child < 0)
        {
            perror("The fork calling was not succesful\n");
            exit(1);
        }

        /* CHILD */
        if (child == 0)
        {
            child_fn();
        }
        /* PARENT */
        msg.mtype = 1;
        sprintf(msg.mtext, "%d %d %s", pot_kell, hanyadik, argv[1]);
        if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }

        pause();

        if (msgrcv(msgqueue, &msg, sizeof(struct message), 2, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }

        printf("Gyerek valasza: '%s'\n", msg.mtext);

        waitpid(child, 0, 0);
        if (strcmp(msg.mtext, "Bevontattak.") || pot_kell)
        {
            hanyadik++;
            pot_kell = 0;
        }
        else
        {
            pot_kell = 1;
            mentesito_db++;
        }
    }

    printf("%d db mentesito jarat kellett\n", mentesito_db);

    return 0;
}

void child_fn()
{
    struct message msg1, msg2;
    int pot_kell, hanyadik, osszes;
    int siker;

    srand (time(NULL) ^ (getpid()<<16));

    if (msgrcv(msgqueue, &msg1, sizeof(struct message), 1, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    sscanf(msg1.mtext, "%d %d %d", &pot_kell, &hanyadik, &osszes);
    siker = rand() % 10;
    if (pot_kell)
        printf("Potszerelveny vagyok, a #%d szerelvenyt mentesitem\n", hanyadik+1);
    else
        printf("En vagyok a #%d szerelveny, lesz meg %d db. %s az uzemet\n", 
            hanyadik+1,
            osszes-hanyadik-1,
            (siker != 0) ? "Sikeresen teljesitettem" 
                        : "Nem teljesitettem sikeresen");

    sleep(1);
    kill(getppid(), SIGUSR1);

    msg2.mtype = 2;
    
    sprintf(msg2.mtext, "%s", (siker != 0) ? "Beertem." : "Bevontattak.");
    if (msgsnd(msgqueue, &msg2, sizeof(struct message), 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    exit(0);
}
