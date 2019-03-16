#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

#define KEY1 2003

void handler(int signumber);

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor,char * buf) 
{      
     struct uzenet uz;
     uz.mtype=5;
     int i;
     buf[i]=getppid();
     int status;
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor,char * buffer ) 
{ 
    struct uzenet uz; 
    int status; 
    status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
    strcpy(buffer,uz.mtext);
    if ( status < 0 ) 
        perror("msgsnd"); 
    else
        printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
    return 0; 
} 


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

    int status; 

    pid_t child_a, child_b;

    child_a = fork();

    if (child_a == 0) {
        /* Child A code */
        // [   UTAS  ]
        sleep(1);
        char* cim = argv[1];
        write(myPipe[1], cim, sizeof(argv[1]));
        printf("[UTAS]: Utazni szeretnek!\n");
        kill(getppid(), SIGTERM);
        pause();
    } else {
        child_b = fork();
        if (child_b == 0) {
            /* Child B code */
            // [   AUTO  ]
            pause();
        } else {
            /* Parent Code */
            // [ KOZPONT ]
            printf("[KOZPONT]: Varok az utas bejelentkezesere!\n");
           // pause();
            read(myPipe[0], buffer, sizeof(argv[1]));
            printf("[KOZPONT]: Utas bejelentkezett, %s-be kuldok autot\n", buffer);
            
            //kill(child_b, SIGTERM);
            //wait(NULL);
        }
    }
    return 0;
}

void handler(int signumber){
    //printf("Megkaptam a %d szamu signalt", signumber);
}