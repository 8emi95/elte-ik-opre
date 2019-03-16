//Bakos Martin
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define KEY 2003

void handler(int signumber);

struct MESSEGE { 
    long mtype;
    char mtext[1024]; 
}; 

int main(int argc, char *argv[]){
	signal(SIGTERM,handler);

    /* Csovezetek letrehozasa */
	char buffer[100];
    int myPipe[2];
	if (pipe(myPipe) < 0)
	{
		perror("\nCsovezetek megnyitasa sikertelen!\n");
		exit(-1);
	}

    /* Üzenetsor létrehozása */
    int msgid = msgget((key_t) KEY, IPC_CREAT | 0666); 
    if ( msgid < 0 ) { 
        perror("msgget"); 
        return 1; 
    } 
    int status; 

    pid_t child_a, child_b;

    child_a = fork();

    if (child_a == 0) {
        /* Child A code */
        // [   UTAS  ]
        sleep(1);
        char* cim = argv[1];
        write(myPipe[1], cim, sizeof(argv[1]));
        printf("[   UTAS  ]: Utazni szeretnek!\n");
        kill(getppid(), SIGTERM);
    } else {
        child_b = fork();
        if (child_b == 0) {
            /* Child B code */
            // [   AUTO  ]
            pause();
            struct MESSEGE uz; 
            status = msgrcv(msgid, &uz, 1024, 5, 0 ); 
            if ( status < 0 ) 
                perror("msgsnd"); 
            else
                printf("[   AUTO  ]: Elindultam az utasert, adatai: %s\n", uz.mtext);
        } else {
            /* Parent Code */
            // [ KOZPONT ]
            printf("[ KOZPONT ]: Varok egy utasra\n");
            pause();
            read(myPipe[0], buffer, sizeof(argv[1]));
            printf("[ KOZPONT ]: Utas bejelentkezett, %s-be kuldok autot\n", buffer);
            
            const struct MESSEGE uz = {5, "PID nem sikerul"}; 
            status = msgsnd( msgid, &uz, sizeof(uz.mtext), 0 ); 
            if ( status < 0 ) 
                perror("msgsnd"); 
            sleep(1);
            
            kill(child_b, SIGTERM);
            wait(NULL);
        }
    }
    return 0;
}

void handler(int signumber){
    //printf("Megkaptam a %d szamu signalt", signumber);
}