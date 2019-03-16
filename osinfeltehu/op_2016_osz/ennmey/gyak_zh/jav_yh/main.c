#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink
// -lrt -vel fordit!

#define READ_END 0
#define WRITE_END 1

int compare_strings(char a[], char b[])
{
   int c = 0;
 
   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
 
   if (a[c] == '\0' && b[c] == '\0')
      return 1;
   else
      return 0;
}

void beert(int signumber){
    if(10==signumber)
    {
        printf("Sikeresen beert a szerelveny.\n");
    }
    else
    {
        printf("Be lett vontatva.\n");
    }
}


void eszrevesz(int signumber){
    printf("Jelzes megkapva, betorok!\n");
}

int main (int argc, char* argv[]) { 

    pid_t init_pid = getpid();
     pid_t child,child_b; 

     char string [50];
    

    struct sigaction partnerArrive;
    partnerArrive.sa_handler = eszrevesz;
    sigemptyset(&partnerArrive.sa_mask);
    partnerArrive.sa_flags = 0;
    sigaction(SIGUSR1,&partnerArrive,NULL);

     int i,db;
     int uzenetsor, status;
	char* mqname="/zarthelyi";
    char* mqname_2="/zarthelyi_2";
	struct mq_attr attr;
	struct mq_attr attr_2;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE
    
	attr_2.mq_maxmsg=5; //MAXMSGS
	attr_2.mq_msgsize=50; //MSGSIZE 
	//
	mq_unlink(mqname); // delete if exist
    mq_unlink(mqname_2); // delete if exist
    
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    mq2=mq_open(mqname_2, O_CREAT|O_RDWR,0600,&attr_2);
    
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     }
     if ( mq2 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
     char buffer[50];	// mq_send, mq_receive uses char array
	 printf("Buffer length: %d\n",strlen(buffer));
     

    int pipefd[2];
    int back_pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

            if (pipe(back_pipefd) == -1)
            {
                perror("Hiba a pipe nyitaskor!");
                exit(EXIT_FAILURE);
            }
     int kevin[2];
     int elso =1;
    child=fork();
    if(child<0) {
				printf("forking error (%d)",i);
				exit(1);
			}
    srand(time(NULL));
    if(init_pid == getpid()) { // parentID
        child_b=fork();
        if(child_b<0) {
				printf("forking error (%d)",i);
				exit(1);
			}
        if(init_pid == getpid()){
            mq_close(mq1);
            mq_close(mq2);
            mq_unlink(mqname);
            int st;
            wait(&st);
            printf("Szulo vegzett (%d)\n",getpid());
        }
        else
        {   
            close(pipefd[WRITE_END]);
            close(back_pipefd[READ_END]);

            printf("Betoro");
            db=mq_receive(mq1,buffer, 50,NULL);
            printf("Hanyatt estem %s jatek miatt.", buffer);
            sprintf(string, "%s","Csak varj, mig a kezunk koze nem kerulsz");
            db=mq_send(mq2,string,50, 4 );
            sleep(1);
            kill(child,SIGUSR1);
            read(back_pipefd[0],string,sizeof(string));
            int hatas =rand()%2;
            if(hatas)
            {

                sprintf(string, "%s","Na megallj csak");
            }
            else
            {
                sprintf(string, "%s","Na megallj csak");
            }
             write(back_pipefd[1], string,sizeof(string));
            mq_close(mq1);
            mq_close(mq2);
            mq_unlink(mqname);
            close(pipefd[READ_END]);
            close(back_pipefd[WRITE_END]);
        }
        int status;
        wait(&status);
	}
	else{

        close(pipefd[READ_END]);
        close(back_pipefd[WRITE_END]);

        sigset_t sigset;
        sigfillset(&sigset);
        sigdelset(&sigset,SIGUSR1);
        sigdelset(&sigset,SIGINT);

         printf("Kevin");
        int aprojatek = (rand()%21) +30 ;
        sprintf(string,"%d",aprojatek);
        db=mq_send(mq1,string,50, 4 );
        sleep(1);
        db=mq_receive(mq2,buffer, 50,NULL);
        printf("%s",buffer);
        sigsuspend(&sigset);
        sprintf(string, "%s", "festek");
        write(pipefd[1], string,sizeof(string));

        mq_close(mq1);
        mq_close(mq2);
        mq_unlink(mqname);

        close(pipefd[WRITE_END]);
        close(back_pipefd[READ_END]);
    }

     return 0; 
} 
