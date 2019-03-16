#include <stdlib.h>  
#include <stdio.h>
#include <sys/ipc.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h> 

struct metroData{
    long mtype;
    int id;
    int max;
    char message[128];
};

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

void sig_handler(){
    printf("A szerelveny uzenete:\n");
}


int main(int argc,char ** argv) {

    pid_t child;
    struct metroData data;
    const static struct metroData reset;

    int msq, status; 
    key_t key; 

    signal(SIGTERM, sig_handler);

    srand(time(NULL));

    key = ftok(argv[0],1); 
     msq = msgget( key, 0600 | IPC_CREAT ); 
     if ( msq < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

    child = fork();

    if(child < 0){
        perror("Fork");
    }else if(child > 0){
        //parent

        data = reset;

        int max = atoi(argv[1]);
        int randnum = rand()%max + 1;

        data.mtype = getpid();
        data.id =  randnum;
        data.max = max;

//        printf("sending\n");
        status = msgsnd(msq, &data, sizeof(data), 0);
        if(status < 0){
            perror("msgsnd");
        }

        //printf("paused\n");
        pause();
        
        //printf("reveiving message\n");
        msgrcv(msq, &data, sizeof(struct metroData), getpid(), 0);
        printf("%s\n", data.message);

        wait(&status);
        status = msgctl( msq, IPC_RMID, NULL ); 
            if ( status < 0 ){
               perror("msgctl");
            }

    }else if(child == 0){
        //child

        data = reset;

        int parentId = getppid();
        int randEvent;

        status = msgrcv(msq, &data, sizeof(struct metroData), parentId, 0);
        if(status < 0){
            perror("msgsnd");
        }

        randEvent = rand()%100;

        //printf("rand %d\n",randEvent);
        if(randEvent > 30){
            printf("A szamom: %d\nEnnyien jonnek meg: %d\nElcsusztam\n", data.id, data.max - data.id, randEvent);
            data = reset;
            strcpy(data.message, "Vontattak");
        }else{
            printf("A szamom: %d\nEnnyien jonnek meg: %d\nSikeres uzem\n", data.id, data.max - data.id,  randEvent);
            data = reset;
            strcpy(data.message, "Beertem magamtol");
        }

        data.mtype = parentId;

        //printf("sending report back\n");
        msgsnd(msq, &data, sizeof(data), 0);
        kill(getppid(), SIGTERM);
    }

    return 0;
}