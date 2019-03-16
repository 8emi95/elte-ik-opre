#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

#define KEY1 2003

void handler(int signumber);

/*struct uzenet { 
     long mtype;
     char mtext[1024]; 
}; 

int kuld( int uzenetsor,char*buffer) 
{ 
     struct uzenet uz;
	 //uz.mtype = buffer[1];
	 strcpy(uz.mtext, buffer[0]);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( " A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} */
 

int main(int argc, char *argv[]){
	signal(SIGTERM,handler);
    /*int uzenetsor, status; 
    key_t kulcs;

	kulcs = ftok(argv[0],1);
	printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } */
    /* Csovezetek letrehozasa */
	char buffer[100];
    int myPipe[2];
	if (pipe(myPipe) < 0)
	{
		perror("\nCsovezetek megnyitasa sikertelen!\n");
		exit(-1);
	}

    pid_t child_a, child_b;
    child_a = fork();

    if (child_a == 0) {
        /* Child A */
        // [UTAS]
        sleep(1);
        char* cim = argv[1];
        write(myPipe[1], cim, sizeof(argv[1]));
        printf("[UTAS]: Utazni szeretnek!\n");
        kill(getppid(), SIGTERM);
        pause();
    } else {
        child_b = fork();
        if (child_b == 0) {
            /* Child B  */
            // [AUTO ]
            pause();
            //fogad( uzenetsor );
            

        } else {
            /* Parent Code */
            // [KOZPONT]
            printf("[KOZPONT]: Varok az utas bejelentkezesere!\n");
            read(myPipe[0], buffer, sizeof(argv[1]));
            printf("[KOZPONT]: Utas bejelentkezett, %s-be kuldok autot\n", buffer);
            /*char buffer[2];

            buffer[0]="Ajka";
            //buffer[1]=getppid();
            sleep(1);
			kuld( uzenetsor,buffer );  // Parent sends a message.
			kill(child_b,SIGTERM);
            wait( NULL ); 
            // After terminating child process, the message queue is deleted. 
            status = msgctl( uzenetsor, IPC_RMID, NULL ); 
            if ( status < 0 ) 
                perror("msgctl"); 
            return 0;*/ 
        }
    }
    return 0;
}

void handler(int signumber){}