#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>
 
struct uzenet {
    long mtype;
    char mtext [ 1024 ];
};

void handler(int signumber){
    printf("Megkapta az uzentet\n",signumber);
}

int kuld( int uzenetsor ) {
    const char *kerdes[] = {"\n-- Ki a Fradi legjobb jatekosa? --\n"};
    struct uzenet uz = { 5, "Hajra Fradi!" };
    strcpy(uz.mtext, kerdes[0]);
     int status;
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );
    if ( status < 0 ) {
        perror("msgsnd");
    }
    return 0;
}

int fogad( int uzenetsor ) {
    struct uzenet uz;
    int status;
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
    if (status < 0) {
        perror("msgsnd");
    }else
        printf( "A kerdes:  %s\n", uz.mtext);
    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int uzenetsor, status;
    key_t kulcs;
       kulcs = ftok(argv[0],1);
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        perror("msgget");
        return 1;
    }
	    signal(SIGTERM, handler);
    pid_t child = fork();
    if (child > 0) {
        pid_t child2 = fork();
        if(child2 == 0) {
            kill(getppid(), SIGTERM);
            sleep(1);
            fogad(uzenetsor);
            printf("1.hallgato \"kicsit gondolkozott\", majd kilepett\n");
        }else {
            pause();
            pause();
            kuld(uzenetsor);
            kuld(uzenetsor);
            wait(NULL);
            status = msgctl( uzenetsor, IPC_RMID, NULL );
            if ( status < 0 ) perror("msgctl");
            int status;
            wait(&status);
        }
		
    }else {
        int i;
        sleep(1);
        kill(getppid(), SIGTERM);
        fogad(uzenetsor);
        printf("1.hallgato \"kicsit gondolkozott\", majd kilepett\n");
    }
	
    return 0;
}
