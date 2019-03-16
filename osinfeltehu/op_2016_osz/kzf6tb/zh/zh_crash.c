#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h> 
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, struct uzenet* uz ) 
{ 
     int status; 
     
     printf("__kuldes: %i, %s\n", uz->mtype, uz->mtext);

     status = msgsnd( uzenetsor, uz, strlen ( uz->mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, struct uzenet* dest, int id ) 
{ 
     int status; 
     status = msgrcv(uzenetsor, dest, sizeof(*dest), id, 0 ); 
     
     if ( status < 0 ) 
     ;
          //perror("msgsnd\n"); 
     else
        ;//printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", dest->mtype, dest->mtext ); 
     return 0; 
} 

void childFunc (int uzenetsor, int szerelvenyCount) {
    struct uzenet idUz;
    fogad(uzenetsor, &idUz, 1);
    printf("[Szerelveny_%s] Elindultam! Ennyi jon meg utanam: %i\n", idUz.mtext, szerelvenyCount-(atoi(idUz.mtext)+1));
    sleep(1); // utazik
    
    struct uzenet myStatus;
    myStatus.mtype = 2;
    if ((rand()%10)+1 <= 20)
        strcpy(myStatus.mtext, "Bevontattak");
    else
        strcpy(myStatus.mtext, "Beertem");

    kuld(uzenetsor, &myStatus);

    kill(getppid(), SIGUSR1);

    exit(0);
}


void usr1Handler(int sn)
{
    printf("[Uzemelteto] Egy szerelveny beerkezett!\n");
}

int main(int argc, char* argv[]) 
{
     int uzenetsor, status; 
     key_t kulcs; 
     
     signal(SIGUSR1, usr1Handler);

     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

    pid_t child;

    if (argc < 2) {
        printf("hianyzo parancssori argumentum\n");
        exit (1);
    }

    int szerelvenyCount = atoi(argv[1]);
    printf("Szerelvenyszam: %i\n", szerelvenyCount);

    //child = fork();
    //if (child == 0) { ///////// child
        //childFunc(uzenetsor, szerelvenyCount);

    //} else { ///////// parent

        int ii;
        for (ii = 0; ii < szerelvenyCount; ++ii) {
            struct uzenet idUz;
            idUz.mtype = 1;

            char strid[4];
            sprintf(strid, "%d", ii );
            strcpy( idUz.mtext, strid);
            kuld(uzenetsor, &idUz);

            child = fork();
            
            if (child == 0) {
                struct uzenet statusUz;
                fogad(uzenetsor, &statusUz, 2);
                printf("[Uzemelteto] Beerkezett szerelveny uzenete: %s\n", statusUz.mtext);
                child = 0;
            } else {
                childFunc(uzenetsor, szerelvenyCount);
            }
        }


        int s;
       wait(&s);
    //}

    return 0;
}