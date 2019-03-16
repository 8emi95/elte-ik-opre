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
int randomszam2();

void handler(int signumber){
  printf("Signal with number %i has arrived\n", signumber);
}

struct uticel {
	int ido;
} uticel;

int main(){
	int max;
	printf("szerelvenyek szama: ");
	scanf("%d:", &max);
	
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
	signal(SIGUSR1,handler);
	
	char* mqname2="/uzenetsor2";
	mqd_t mq2;

	mq_unlink(mqname2); // delete if exist
	mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr);// attr azonos
     if ( mq2 < 0 ) { 
        perror("MQ hiba");
        return 1; 
    }
	
	key_t kulcs;
    int oszt_mem_id;
    struct uticel osztMemo[4];
    struct uticel *osztMemoPtr;

    kulcs = ftok("shm.txt",1);
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    osztMemoPtr = (struct uticel*)shmat(oszt_mem_id,NULL,0);
	
	
	srand(time(NULL));
	


    pid = fork();	// ha nulla akor gyerek, negatív hiba, más szülő
    if (pid == -1) {
		perror("Fork hiba");
        exit(EXIT_FAILURE);
   }
	
	if (pid == 0) { //gyerek
		char sorsz[100];
		int bv = mq_receive(mq1,sorsz, 100, NULL);
		int elromlott = -1;
		sorsz[bv]= '\0';
		printf("szerelvenyszam: %s\n", sorsz);
		int r = randomszam();
		kill(getppid(),SIGUSR1);
		if(r<=10){
			mq_send(mq2,"bananhej", 8, 4);
			osztMemo[1].ido = -1;
		}else if(r>10 && r<=20) {
			mq_send(mq2,"lefagyott", 9, 4);
			osztMemo[1].ido = -1;
		} else {
			mq_send(mq2,"siker", 5, 4);
			int ido2 = randomszam2();
			osztMemo[1].ido = ido2;
		}
		

		
		
    } else { //szülő
		char eredmeny[100];
		mq_send(mq1,"1.szereveny", 11, 4);
		pause(); //waits till a signal arrive 
		printf("Signal arrived, szerlveny beert ",SIGTERM);
		int br = mq_receive(mq2,eredmeny, 100, NULL);
		eredmeny[br] = '\0';
		printf("%s\n", eredmeny);
		printf("%d perc\n", osztMemoPtr[1].ido);
		wait(NULL);
	}
	
	
}


int randomszam(){
  int r=rand()%100; 
  return r+1;
}

int randomszam2(){
  int r=rand()%100; 
  return r+11;
}