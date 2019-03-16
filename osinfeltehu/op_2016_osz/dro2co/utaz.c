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

#define MEMSIZE 1024

int szemafor_letrehozas(const char* pathname,int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}


void szemafor_muvelet(int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if(semop(semid,&muvelet,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void szemafor_torles(int semid){
      semctl(semid,0,IPC_RMID);
}


int gyerekKesz = 0;

void handler(int signumber){
  printf("Signal with number %i has arrived\n", signumber);
  gyerekKesz++;
}
int randomvar();
int randomcel();

struct uticel {
	char nev[100];
	int hely;
} uticel;

int main(){

	signal(SIGUSR1,handler);

	pid_t pid; //gyerek - 0 /nemgyerek 
	char bufi[100]; //buffer
	srand(time(NULL));

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

    // osz. mem
    key_t kulcs;
    int oszt_mem_id;
    struct uticel osztMemo[4];
    struct uticel *osztMemoPtr;

    kulcs = ftok("shm.txt",1);
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    osztMemoPtr = (struct uticel*)shmat(oszt_mem_id,NULL,0);

    // 2. queue visszafele a parent fele
    	//queue
	char* mqname2="/uzenetsor2";
	mqd_t mq2;

	mq_unlink(mqname2); // delete if exist
	mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr);// attr azonos
     if ( mq2 < 0 ) { 
        perror("MQ hiba");
        return 1; 
    }

    // semafor a foglalashoz
    int semid = szemafor_letrehozas("sem.txt",1); //up -> unlocked


    pid = fork();	// ha nulla akor gyerek, negatív hiba, más szülő
   if (pid == -1) {
       perror("Fork hiba");
       exit(EXIT_FAILURE);
   }

	
	if (pid == 0) {
		// gyerek1
		printf("Gy1\n");
		kill(getppid(),SIGUSR1);
		srand(time(NULL) - 1*2); //reset rand. seed
		char bufi1[100];
		int br = mq_receive(mq1,bufi1, 100, NULL);
		bufi1[br] = '\0';
		printf("Gy1: %s\n", bufi1);
		sleep(randomvar());

		int cel1 = randomcel();
		printf("%d. %s - %d\n", cel1, osztMemoPtr[cel1].nev, osztMemoPtr[cel1].hely);

		//Foglalas
		szemafor_muvelet(semid,-1); // krit. kezd
		if (osztMemoPtr[cel1].hely > 0) {
			printf("Gy1: van meg hely, foglal\n");
			mq_send(mq2,osztMemoPtr[cel1].nev, strlen(osztMemoPtr[cel1].nev), 4);
		} else {
			printf("Gy1: nincs hely\n");
		}
		szemafor_muvelet(semid,1); // krit. vege


	} else {
		pid_t pid2 = fork();
		if (pid2 == 0) {
			// gyerek2
			printf("Gy2\n");
			kill(getppid(),SIGUSR1);
			srand(time(NULL) - 2*2); //reset rand. seed
			char bufi2[100];
			int br = mq_receive(mq1,bufi2, 100, NULL);
			bufi2[br] = '\0';
			printf("Gy2: %s\n", bufi2);
			sleep(randomvar());

			int cel2 = randomcel();
			printf("%d. %s - %d\n", cel2, osztMemoPtr[cel2].nev, osztMemoPtr[cel2].hely);

			//Foglalas
			szemafor_muvelet(semid,-1); // krit. kezd
			if (osztMemoPtr[cel2].hely > 0) {
				printf("Gy2: van meg hely, foglal\n");
				mq_send(mq2,osztMemoPtr[cel2].nev, strlen(osztMemoPtr[cel2].nev), 4);
			} else {
				printf("Gy2: nincs hely\n");
			}
			szemafor_muvelet(semid,1); // krit. vege

		} else {
			pid_t pid3 = fork();
			if (pid3 == 0) {
				// gyerek3
				printf("Gy3\n");
				kill(getppid(),SIGUSR1);
				srand(time(NULL) - 3*2); //reset rand. seed
				char bufi3[100];
				int br = mq_receive(mq1,bufi3, 100, NULL);
				bufi3[br] = '\0';
				printf("Gy3: %s\n", bufi3);
				sleep(randomvar());

				int cel3 = randomcel();
				printf("%d. %s - %d\n", cel3, osztMemoPtr[cel3].nev, osztMemoPtr[cel3].hely);

				//Foglalas
				szemafor_muvelet(semid,-1); // krit. kezd
				if (osztMemoPtr[cel3].hely > 0) {
					printf("Gy3: van meg hely, foglal\n");
					mq_send(mq2,osztMemoPtr[cel3].nev, strlen(osztMemoPtr[cel3].nev), 4);
				} else {
					printf("Gy3: nincs hely\n");
				}
				szemafor_muvelet(semid,1); // krit. vege

			} else {
				// szuloooo
				printf("Szulo var...\n");
				
				while(gyerekKesz < 3) {
					sleep(1);
				}
				printf("Szulo kuld\n");
				mq_send(mq1,"2016:11:08",10, 4);
				mq_send(mq1,"2016:11:08",10, 4);
				mq_send(mq1,"2016:11:08",10, 4);

				//osztott meg iras
				//struct uticel c1;
				strcpy(osztMemo[0].nev, "Los-Angeles");
				osztMemo[0].hely = 15;

				//struct uticel c2;
				strcpy(osztMemo[1].nev, "Miami");
				osztMemo[1].hely = 5;

				//struct uticel c3;
				strcpy(osztMemo[2].nev, "Florida");
				osztMemo[2].hely = 8;

				//struct uticel c4;
				strcpy(osztMemo[3].nev, "New-York");
				osztMemo[3].hely = 11;

				//osztMemo[0] = c1;
				//osztMemo[1] = c2;
				//osztMemo[2] = c3;
				//osztMemo[3] = c4;
				//memcpy(osztMemoPtr, c1, sizeof(c1));
				//memcpy(osztMemo, &c2, sizeof(c2));
				//memcpy(osztMemo, &c3, sizeof(c3));
				//memcpy(osztMemo, &c4, sizeof(c4));
				memcpy(osztMemoPtr, osztMemo, sizeof(osztMemo));

				// Foglalasok kezelese
				int i = 0;
				int j;
				int readLen;
				for (i=0; i<3; i++) {
					readLen= mq_receive(mq2, bufi, 100, NULL);
					bufi[readLen] = '\0';
					printf("Foglalas ide: %s\n", bufi);

					for (j = 0; j<4; j++){ //tenyleges foglalas
						if (strcmp(osztMemoPtr[j].nev, bufi) == 0) {
							osztMemoPtr[j].hely--;
							printf("Nev %s maradek hely: %d\n", bufi, osztMemoPtr[j].hely);
						}
					}
				}
				

				waitpid(pid, NULL, 0);
				waitpid(pid2, NULL, 0);
				waitpid(pid3, NULL, 0);
				printf("Szulo vege\n");
				shmctl(oszt_mem_id,IPC_RMID,NULL);
			}
		}
	}


	shmdt(osztMemo);
	mq_close(mq1);
	mq_unlink(mqname);
	mq_close(mq2);
	mq_unlink(mqname2);
}

int randomvar(){
  int r=rand()%1; 
  return r+2;
}

int randomcel(){
  int r=rand()%4; 
  return r;
}