#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>

struct Message { 
     long mtype;
     int aktSzam;
     int maxSzam; 
}; 

struct HibaOk{
     long mtype;
     char ok[100];
};

void handler(int signumber){
  printf("[Sys] Valaki szignalt kuldott %i szammal\n",signumber);
}


int hibaSend(int mq, struct HibaOk qu)
{ 
     int status;
     status = msgsnd( mq, &qu, sizeof(qu) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int hibaReceive(int mq, struct HibaOk* qu) 
{ 
     int status; 
     status = msgrcv(mq, qu, sizeof(*qu), 0, 0 ); 
	 printf("[SYS] %Uzenetkod: ld , %s \n",qu->mtype, qu->ok);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

int mqSend(int mq, struct Message qu)
{ 
     int status;
     status = msgsnd( mq, &qu, sizeof(qu) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct Message* qu) 
{ 
     int status; 
     status = msgrcv(mq, qu, sizeof(*qu), 0, 0 ); 
	 printf("[SYS] %Uzenetkod: ld , %d , %d\n",qu->mtype, qu->aktSzam, qu->maxSzam);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void metro()
{
    
}

int main(int argc, char** argv)
{
    int maxSzSzam = atoi(argv[1]);

    signal(SIGTERM,handler);

   // printf(" A szerelvenyszam: %d \n", maxSzSzam);

    int messageQueue, hibaQueue, status;
    key_t key;

    key = ftok(argv[0], 2);
    messageQueue = msgget( key, 0600 | IPC_CREAT);
    hibaQueue = msgget( key, 0600 | IPC_CREAT);

    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 

    pid_t szerelveny;

    szerelveny = fork();

    if (szerelveny < 0)
    {
        perror("Hiba a szerelveny Forkolasakor \n");
        exit(EXIT_FAILURE);
    }
    //Szerelveny
    if ( szerelveny == 0 )
    {   
        int alive = rand() % 100;
        struct Message ms;
        mqReceive(messageQueue, &ms);
        printf("[SZERELVENY] En a %d metro vagyok, maximum metroszam: %d \n", ms.aktSzam, ms.maxSzam);
        struct Message ho = {3,2,2};
      if ( alive < 10 )
        { 
            printf ("[SZERELVENY] Meghibasodtam napkozben \n");
            ho.aktSzam = 0;
        }
        else
        {
            printf("[SZERELVENY] Sikeresen teljesitettem az uzemet \n ");
            ho.aktSzam = 1;
        }
        sleep(1);        
        kill(getppid(),SIGTERM); 

        mqSend(messageQueue, ho);
        printf(" [SZERELVENY] Szerelveny vegzett\n");        
    }
    else{ // Szulo

        int actSzam = rand() % maxSzSzam + 1;
        struct Message ms = {2, actSzam, maxSzSzam};
        mqSend(messageQueue, ms);
        pause();
        printf("[BMF] Szerelveny szolt hogy visszaert \n");

        struct Message ho;
        mqReceive(messageQueue, &ho);
        if( ho.aktSzam == 0)
        {
            printf("[BMF] Hiba tortent \n");
        }
        else
        {
            printf("[BMF] Sikeresen visszaert \n ");
        }
        
        int status;
        waitpid(szerelveny, &status, 0);
        status = msgctl( messageQueue, IPC_RMID, NULL ); 
        if ( status < 0 ) 
            perror("msgctl"); 



        printf("[BMF] Szulo vegzett \n ");
    }

    return 0;
}