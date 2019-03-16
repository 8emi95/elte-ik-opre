#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	 //int szam[2]; 
	 int szam;
};

struct szovuzenet{
	long mtype;
	char mtext[1024];
};

int i = 0;
int romlott = 0;
void handler(int signumber){
	if(signumber == SIGRTMIN + 1){
		romlott = 1;
	}
	i++;
}

int main(int argc, char** argv){

	int uzenetsor;
	int* o_ptr;
	int o_id=shmget(IPC_PRIVATE,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    uzenetsor = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
	o_ptr = shmat(o_id,NULL,0);
	if(uzenetsor < 0){
		perror("msgget");
		return 1;
	}
	
	
	int j;
	for(j = 0; j < atoi(argv[1]); j++){
		pid_t child;
		child = fork();
		if(child < 0){
			perror("gyerek letrehozasa");
			exit(1);
		}
		if(child > 0){//parent
			i = 0;
			romlott = 0;
			srand(getpid());
			struct sigaction sigact;
			sigact.sa_handler=handler; 
			sigemptyset(&sigact.sa_mask);     
			sigact.sa_flags=0;
			sigaction(SIGRTMIN,&sigact,NULL);
			sigaction(SIGRTMIN+1,&sigact,NULL);
			
			int szerelvenyszam = j + 1;
			struct uzenet uz;
			uz.mtype = child;
			//strcpy(uz.mtext, argv[1]);	
			uz.szam = atoi(argv[1]);
			
			int status = msgsnd(uzenetsor, &uz, sizeof(int) , 0 );
			uz.szam = szerelvenyszam;
			status  = msgsnd(uzenetsor, &uz, sizeof(int), 0);
			if(status < 0)perror("msgsnd");
			while(i == 0){usleep(100);}
			struct szovuzenet valasz;
			status = msgrcv(uzenetsor, &valasz, 1024, getpid(), 0 ); 
			printf("A szerelveny kuldi a hibat/nemhibat: %s\n\n", valasz.mtext);
			
			
		}else{//child
			struct uzenet uz;
			int status = msgrcv(uzenetsor, &uz, sizeof(int), getpid(), 0 ); 
			int max = uz.szam;
			status = msgrcv(uzenetsor, &uz, sizeof(int), getpid(), 0 );
			
			int mennyi = uz.szam;
			srand(getpid());
			int elromlott = rand() % 10;
			printf("%i. szerelveny, meg %i szerelveny van hatra ", mennyi, max - mennyi);
			struct szovuzenet valasz;
			valasz.mtype = getppid();
			
			int utazasiido;
			
			if(elromlott >= 2){
				printf("es nem is romlottam el :)))) \n");
				strcpy(valasz.mtext, "Nincs baj");
				kill(getppid(), SIGRTMIN);
				
				utazasiido = rand() % 20 + 11;
				o_ptr[0] = mennyi;
				o_ptr[1] = utazasiido;
				
			}else{
				printf(" es sajnos elromlottam :(((((((\n");
				if(elromlott == 0){
					strcpy(valasz.mtext, "Bananhej");
				}
				kill(getppid(), SIGRTMIN+1);
				o_ptr[0] = mennyi;
				o_ptr[1] = -1;
			}
			
			status = msgsnd(uzenetsor, &valasz, strlen(valasz.mtext) + 1, 0);
			 
			if ( status < 0 ) 
				perror("msgsnd"); 

		
			
		
			exit(0);
		}
	}
	return 0;
}