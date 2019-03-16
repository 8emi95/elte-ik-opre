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

typedef struct{
    int m_id;
    int m_max;
} MetroData;

void exitHandler(){
    printf("Vege\n");
}

int sendData( int message, int pid, int id, int max ) 
{ 
     int status; 
     MetroData data;
     data.m_id = id;
     data.m_max = max;
     
     status = msgsnd( message, &data, sizeof(data) , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int recData( int message, int pid ) 
{ 
     MetroData data; 
     int status; 
     int metros_left;
     status = msgrcv(message, &data, 8, pid, 0 ); 
     
     if ( status < 0 ) {
          perror("msgrcv"); 
     }
     else{
          metros_left = data.m_max - data.m_id;
           int rand_event = (rand()%100);
if(rand_event > 90){
            printf("Szerelvenyem szama: %d\nEnnyien lesznek utanam: %d\nBevontattak mert elcsusztam %d\n", data.m_id, metros_left, rand_event);
        }else{
            printf("Szerelvenyem szama: %d\nEnnyien lesznek utanam: %d\nMagamtol elertem a vegallomast %d\n",data.m_id, metros_left, rand_event);
        } 
     }
     return 0; 
} 

int send( int message, int pid, char *s ) 
{ 
     int status; 
     char msg[15];
     strcpy(msg, s);
     
     status = msgsnd( message, msg, 15 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int rec( int message, int pid ) 
{ 
     int status; 
     char s[15];

     status = msgrcv(message, s, 15, pid, 0 ); 
     
     if ( status < 0 ) {
          perror("msgrcv"); 
     }
     printf("%s", s);
     return 0; 
} 

void childEventHandler(){
    printf("Uzenet a szerelvenytol: ");
}

int main(int argc,char ** argv) {

    pid_t child;

     pid_t parent = getpid();
     int message; 
     int m_status;
     key_t m_key; 

     struct sigaction sigact;
            sigact.sa_handler=childEventHandler;
            sigemptyset(&sigact.sa_mask);
            sigact.sa_flags=0;
            sigaction(SIGUSR1,&sigact,NULL);
     
     m_key = ftok(argv[0],1); 
     message = msgget( m_key, 0600 | IPC_CREAT ); 
     if ( message < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

    static const MetroData resetData;


    srand(time(NULL));
    signal(SIGTERM,exitHandler);

    child = fork();

    if(child < 0){
        perror("Fork");
    }else if(child > 0){ //parent

    int MAX = atoi(argv[1]);
    int rand_m_id = (rand()%MAX) + 1;
    int status;
    MetroData data = resetData;

    data.m_id = rand_m_id;
    data.m_max = MAX;

    sendData( message, child, data.m_id, MAX);
    rec(message, parent);
    wait(&status);
    status = msgctl( message, IPC_RMID, NULL ); 

    }else{ //child
        MetroData data = resetData;
        int status;
        
        recData(message, child);
        send(message, parent, "Elcsusztam");
       // raise(SIGUSR1);
        kill(getppid(),SIGTERM);
    }
}