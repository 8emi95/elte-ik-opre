//Vitányi Anna, DRO2CO
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/shm.h>


int randomszam();

int main(){

	pid_t pid; //gyerek - 0 /nemgyerek 
	char sz[100]; //ez megy a pipeban
	
	//queue
	char* mqname="/uzenetsor1";
	struct mq_attr attr;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=100; //MSGSIZE 
	mqd_t mq1;
	
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	if ( mq1 < 0 ) { 
		perror("MQ hiba");
		return 1; 
    }
	
	char* mqname2="/uzenetsor2";
	mqd_t mq2;

	mq_unlink(mqname2); // delete if exist
	mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr);// attr azonos
     if ( mq2 < 0 ) { 
        perror("MQ hiba");
        return 1; 
    }
	
	srand(time(NULL));
	


    pid = fork();	// ha nulla akor gyerek, negatív hiba, más szülő
    if (pid == -1) {
		perror("Fork hiba");
        exit(EXIT_FAILURE);
   }
	
	if (pid == 0) { //gyerek
		char sorsz[100];
		int bv = mq_receive(mq1,sorsz, 100, NULL);
		sorsz[bv]= '\0';
		printf("szerelvenyszam: %s\n", sorsz);
		int r = randomszam();
		if(r<=10){
			mq_send(mq2,"bananhej", 8, 4);
		}else if(r>10 && r<=20) {
			mq_send(mq2,"lefagyott", 9, 4);
		} else {
			mq_send(mq2,"siker", 5, 4);
		}

		
		
		
		
    } else { //szülő
		char eredmeny[100];
		mq_send(mq1,"1.szereveny", 11, 4);
		int br = mq_receive(mq2,eredmeny, 100, NULL);
		eredmeny[br] = '\0';
		printf("allomas: %s\n", eredmeny);
		wait(NULL);
	}
	
	
}



int randomszam(){
  int r=rand()%100; 
  return r+1;
}