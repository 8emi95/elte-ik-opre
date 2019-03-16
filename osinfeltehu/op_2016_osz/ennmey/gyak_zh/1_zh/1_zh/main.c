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
// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink
// -lrt -vel fordit!


int compare_strings(char a[], char b[])
{
   int c = 0;
 
   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
 
   if (a[c] == '\0' && b[c] == '\0')
      return 0;
   else
      return -1;
}

void beert(int signumber){
  printf("Beert rendben\n");
}


void beert2(int signumber){
    printf("Be lett vontatva(nagy tel miatt)\n");
    if(signumber=1)
    {
        printf("Be lett vontatva(nagy tel miatt)\n");
    }
    else
    {
        printf("Be lett vontatva(nagy tel miatt)\n");
    }
}

int main (int argc, char* argv[]) { 

    int max = atoi(argv[1]);
    srand(time(NULL));

    struct sigaction partnerArrive;
    partnerArrive.sa_handler = beert;
    sigemptyset(&partnerArrive.sa_mask);
    partnerArrive.sa_flags = 0;
    sigaction(SIGUSR1,&partnerArrive,NULL);
    
    /*struct sigaction partnerArrive2;
    partnerArrive2.sa_handler = beert2;
    sigemptyset(&partnerArrive2.sa_mask);
    partnerArrive2.sa_flags = 0;
    sigaction(SIGUSR2,&partnerArrive2,NULL);*/
    
     pid_t child; 
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
     char buffer[100];	// mq_send, mq_receive uses char array
	 printf("Buffer length: %d\n",strlen(buffer));


     child = fork(); 
     if ( child > 0 ) { 
         
        sigset_t signalsetting;
        sigfillset(&signalsetting);
        sigdelset(&signalsetting,SIGUSR1);
         printf("Parent process starts\n");
        /*
        sigset_t signalset;
        sigfillset(&signalset);
        sigdelset(&signalset,SIGUSR2);
        sigsuspend(&signalset);*/
    
        char str[30];
        int hanyadik = rand()%max;
        sprintf(str, "%d", hanyadik);
        int db=mq_send(mq1,str,2, 4 );  //Parent sends a message. 
        sprintf(str, "%d", max-hanyadik);
        db=mq_send(mq1,str,2, 4 );  //Parent sends a message.
                
        db=mq_receive(mq2,buffer, 54,NULL);
        printf("A szerelvennyel a kovetkezo tortenik: %s \n",buffer);
        
        sigsuspend(&signalsetting);
        
        int status;
        wait(&status);
        
        mq_close(mq1);
        mq_close(mq2);
        mq_unlink(mqname);
        printf("Parent process ended\n");
     }
    else
    { // child
    char str[30];
	sleep(1); 
	printf("Child process starts\n");
    
    int db=mq_receive(mq1,buffer, 54,NULL);
	printf("En a(z) : %s szerelveny vagyok\n",buffer, db,strerror(errno));

    db=mq_receive(mq1,buffer, 50, NULL);
	printf("Meg ennyien jonnek utana:%s\n", buffer,db);
    
    int beert_e = rand()%2;
    sprintf(str, "%d", beert_e);
    
    if(beert_e)
     {
            printf("Beertem, siker.\n");
     }
    else
    {
        printf("Sajnos bevontattak.\n");
    }
    
    int bananhej = rand()%2;
    sprintf(str, "%d", bananhej);
    if(!beert_e && bananhej)
    {
        char * result_str = "bananhej";
    }
    else if(!beert_e)
    {
        char * result_str = "rideg tel";
    } 
    db=mq_send(mq2,result_str,15, 1 );  //Child sends a message.
     kill(getppid(),SIGUSR1);



    printf("Child process ends.\n");
    mq_close(mq1);
    mq_close(mq2);
    mq_unlink(mqname_2);
	//return 0; 
     }
     
     return 0; 
} 
