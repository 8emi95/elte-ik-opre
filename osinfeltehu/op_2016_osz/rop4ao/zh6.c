#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

struct uzenet {
    long mtype;
    long maxSz;
    char msg[1024];
};

static int beert = 0;

int kuld( int uzenetsor, struct uzenet u)
{
    int status;

    status = msgsnd( uzenetsor, (char*)&u, sizeof(u),0);
    if (status < 0)
        perror("Msgsnd hiba");
    return 0;
}

int fogad( int uzenetsor, struct uzenet* u)
{
    int status;

    status = msgrcv(uzenetsor, u, sizeof(*u), 0, 0);
    return status;
}

void success(int signumber){
    beert = 1;
    printf("Sikeresen beert a szerelveny\n");
}

void fail(int signumber){
    beert = 0;
    printf("Az inditott szerelveny sikertelen", signumber);
}

int main(int argc, char* argv[])
{
    const int max_szerelveny = atoi(argv[1]);
    pid_t child;
    int uzenetsor, status;
    key_t key;

    signal(SIGUSR1,success);
    signal(SIGUSR2,fail);

    srand(time(NULL));

    key = ftok("/tmp",1);
    uzenetsor = msgget(key, 0600 | IPC_CREAT);
    if(uzenetsor < 0)
    {
        perror("Msgget hiba");
        return 1;
    }

    child = fork();
    if(child > 0)
    {
        long szerelvenyszam = rand()%max_szerelveny+1;
        struct uzenet u = {szerelvenyszam, max_szerelveny, "Inditom a vonatot!!"};
        kuld(uzenetsor,u);
        pause();
        if(beert)
            return 0;

        if ( fogad(uzenetsor,&u) < 0)
            perror("Msgsnd hiba");
        else
            printf("Az uzenet kodja: %ld, szovege: %s\n", u.mtype, u.msg);

        status = msgctl(uzenetsor, IPC_RMID, NULL);
        if (status < 0)
            perror("Msgctl hiba");
        return 0;
    } else if (child == 0)
    {
        struct uzenet u;
        do {
            if (fogad(uzenetsor, &u) < 0)
                perror("Msgsend hiba");
            else
                printf("En a szerelveny %ld vagyok, utanam meg %ld szerelveny jon\n", u.mtype, (u.maxSz-u.mtype), u.msg);

            int r=rand()%101;
            if(r>=90)
            {
                kill(getpid(),SIGUSR2);
                int reason = rand()%2;
                if (reason == 0)
                    strcpy(u.msg, "Elcsusztam a bananon:(");
                else
                    strcpy(u.msg, "Lefagyott a sin");
                kuld(uzenetsor, u);
            } else
            {
                kill(getpid(),SIGUSR1);
            }
        } while (u.maxSz-u.mtype > 0);
    } else
    {
        perror("fork hiba");
        return 1;
    }
    return 0;
}
