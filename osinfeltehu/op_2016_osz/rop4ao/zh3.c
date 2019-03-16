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

struct uzenet {
    long mtype;
    char msg[1024];
};

int kuld( int uzenetsor, struct uzenet u) {
    int status;

    status = msgsnd( uzenetsor, &u, strlen(u.msg)+1,0);
    if (status < 0)
        perror("Msgsnd hiba");
    return 0;
}

int fogad( int uzenetsor, struct uzenet* u) {
    int status;

    status = msgrcv(uzenetsor, u, 1024, 0, 0);
    return status;
}

int main(int argc, char* argv[]) {
    pid_t child;
    int uzenetsor, status;
    key_t key;
    const long szerelvenyszam = atoi (argv[1]);

    srand(time(NULL));

    key = ftok(argv[0],1);
    uzenetsor = msgget(key, 0600 | IPC_CREAT);
	
    if(uzenetsor < 0) {
        perror("Msgget hiba");
        return 1;
    }

    child = fork();
	
    if(child > 0) {
        int r = rand()%101;
        struct uzenet u = {szerelvenyszam, "Indul a vonat"};
        kuld(uzenetsor,u);
        wait(NULL);
        if ( fogad(uzenetsor,&u) < 0)
            perror("Msgsnd hiba");
        else
            printf("Gyerek írja: %ld, szovege: %s\n", u.mtype, u.msg);

        status = msgctl(uzenetsor, IPC_RMID, NULL);
        if (status < 0)
            perror("Msgctl hiba");
        return 0;
    } else if (child == 0) {
        struct uzenet u;
        if (fogad(uzenetsor, &u) < 0)
            perror("Msgsend hiba");
        else
            printf("Szulo írja: %ld, szovege: %s\n", u.mtype, u.msg);

        int r=rand()%101;
		
        if(r>=90) {
            strcpy(u.msg, "Elcsusztam a bananhejon!");
        } else {
            strcpy(u.msg, "Sikeresen beertem!");
        }
		
        kuld(uzenetsor, u);
    } else {
        perror("fork hiba");
        return 1;
    }
    return 0;
}
