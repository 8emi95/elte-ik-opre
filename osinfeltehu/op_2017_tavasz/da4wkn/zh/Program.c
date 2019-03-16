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

struct mymsg { 
     long mtype;
     char mytext[1000]; 
}; 

int mqSend(int mq, struct mymsg msg)
{ 
     int status;
     status = msgsnd( mq, &msg, sizeof(msg) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct mymsg* msg) 
{ 
     int status; 
     status = msgrcv(mq, msg, sizeof(*msg), 0, 0 ); 
	//  printf("%ld %s\n",msg->mtype, msg->mytext);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void ParentProc(int utasPid, int taxiPid, int msgQueue,  int* utasPipe)
{
	//parent
	
    int status;
	//pipe:
	close(utasPipe[1]);
    
    while(1)
    {
        char cim[100] ;
        if (read(utasPipe[0], &cim, sizeof(cim)) != 0)
        {
            printf("KOZPONT: Uj cim jelentkezett be: %s \n", &cim);

            struct mymsg msg;// = { utasPid, &cim } //{ utasPid, cim };
            msg.mtype = utasPid;
            strcpy(msg.mytext, cim);
            mqSend(msgQueue, msg);
            printf("KOZPONT: A taxi elindult. Cim: %s, Tel:  %ld \n", msg.mytext, msg.mtype);
            
            // mqReceive(TtoK, &msg);
            //waiting for the childs to terminate:
            waitpid(utasPid,&status,0); 
            printf("KOZPONT: Utas terminalt.\n");
            if ( status < 0 ) 
                perror("utas"); 
            waitpid(taxiPid,&status,0); 
            printf("KOZPONT: Taxi terminalt.\n");
            if ( status < 0 ) 
                perror("taxi"); 

        }
        printf("KOZPONT: Ujabb utasra varok. \n");
        sleep(3);
    }
    //delete mq:
    status = msgctl( msgQueue, IPC_RMID, NULL ); 
    if ( status < 0 ) 
        perror("msgctl"); 
	//pipe:
	close(utasPipe[0]);
}

void UtasProc(int* utasPipe)
{
    
    printf("UTAS: Taxit kerek.\n");
	//seed
	// srand(time(NULL) + 100 * getpid());
    
	//pipe
	close(utasPipe[0]);
	// int r = (rand() % 4) + 1;
    char r[100] = "Pazmany Peter setany 1/c";
	write(utasPipe[1], &r, sizeof(r));
	
	sleep(1);
}

void TaxiProc(int msgQueue)
{
    printf("TAXI: Elkezdtem dolgozni.\n");
	//seed
	// srand(time(NULL) + 100 * getpid());
	struct mymsg msg;
	mqReceive(msgQueue, &msg);
	printf("TAXI: Uj ugyfelemhez indulok. Cim: %s, Tel:  %ld \n", msg.mytext, msg.mtype);
    
    
    msg.mtype = 2;
    mqSend(msgQueue, msg);
    

	sleep(1);
}

int main(int argc, char* argv[])
{
    int msgQueue, status; 
    key_t key;
	//get key
	key = ftok(argv[0],2); 
	//create mq
	msgQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( msgQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	//create pipe
	int utasPipe[2];
	if (pipe(utasPipe) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid_t utas = fork(); 
	
	if (utas < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(utas > 0)
	{
		pid_t taxi = fork();
		if (taxi < 0)
		{
			perror("fork"); 
		}
		else if(taxi > 0)
		{
			//parent
			ParentProc(utas, taxi, msgQueue,  utasPipe);
		}
		else
		{
			//taxi
            printf("TaxiProc inditasa.\n");
			TaxiProc(msgQueue);
		}
		
	}
	else
	{
		//utas
        UtasProc(utasPipe);

	}
	
}
