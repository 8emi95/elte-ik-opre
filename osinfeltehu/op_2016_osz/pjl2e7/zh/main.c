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
int gyerekelromlott = 0;
void handler(int signumber){
    i++;
	if(signumber == SIGRTMIN + 1){
		gyerekelromlott = 1;
	}
}

int main(int argc, char** argv){

	char* sem_nev="/pjl2e7";
	sem_t *semid=sem_open(sem_nev,O_CREAT,S_IRUSR|S_IWUSR,1 );
	if (semid==SEM_FAILED)
	perror("sem_open");
	int osszido = 0;
	int uzenetsor;
	int mentesitok = 0;
	int* oszt_mem_ptr;
	int oszt_mem_id=shmget(IPC_PRIVATE,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    uzenetsor = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
	oszt_mem_ptr = shmat(oszt_mem_id,NULL,0);
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
			gyerekelromlott = 0;
			srand(getpid());
			struct sigaction sigact;
			sigact.sa_handler=handler; 
			sigemptyset(&sigact.sa_mask);     
			sigact.sa_flags=0;
			sigaction(SIGRTMIN,&sigact,NULL);
			sigaction(SIGRTMIN+1,&sigact,NULL);
			
			int szerelvenyszam = j + 1;//rand() % atoi(argv[1]) + 1;
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
			
			printf("A szerelveny valasza: %s\n", valasz.mtext);
			
			sem_wait(semid);
			if(oszt_mem_ptr[1] != -1){
				osszido += oszt_mem_ptr[1];
			}
			sem_post(semid);
			
			if(status < 0)perror("msgrcv");
			
			if(gyerekelromlott == 1){
				pid_t potchild;
				mentesitok++;
				potchild = fork();
				if(potchild < 0){
					perror("gyerek letrehozasa");
					exit(1);
				}
				if(potchild > 0){//szulo
					i = 0;
					struct uzenet uz;
					uz.mtype = potchild;
					uz.szam = -1;
					int status = msgsnd(uzenetsor, &uz, sizeof(int), 0);
					uz.szam = szerelvenyszam;
					status = msgsnd(uzenetsor, &uz, sizeof(int), 0);
					while(i == 0){
						usleep(100);
					}
					sem_wait(semid);
					if(oszt_mem_ptr[1] != -1){
						
						osszido += oszt_mem_ptr[1];
						
					}
					sem_post(semid);
					
					
					
				}else{//potgyerek
					struct uzenet uz;
					int max;
					int hanyadik;
					int status = msgrcv(uzenetsor, &uz, sizeof(int), 0);
					max = uz.szam;
					status = msgrcv(uzenetsor, &uz, sizeof(int), 0);
					hanyadik = uz.szam;
					
					if(max == -1){
						printf("A %i. jaratot helyettesitem\n", hanyadik);
					}
					
					srand(getpid());
					
					int elromlott = rand() % 10;
					sem_wait(semid);
					if(elromlott < 2){
						oszt_mem_ptr[0] = hanyadik;
						oszt_mem_ptr[1] = -1;
					}else{
						oszt_mem_ptr[0] = hanyadik;
						oszt_mem_ptr[1] = rand() % 20 + 11;
					}
					sem_post(semid);
					
					kill(getppid(), SIGRTMIN);
					exit(0);
					
				}
				
				
			}
			
		}else{//child
			struct uzenet uz;
			int status = msgrcv(uzenetsor, &uz, sizeof(int), getpid(), 0 ); 
			int max = uz.szam;
			status = msgrcv(uzenetsor, &uz, sizeof(int), getpid(), 0 );
			
			int hanyadik = uz.szam;
			
			srand(getpid());
			 
			int elromlott = rand() % 10;
			
			printf("%i. vagyok, meg %i van, ", hanyadik, max - hanyadik);
			
			struct szovuzenet valasz;
			valasz.mtype = getppid();
			
			int utazasiido;
			
			sem_wait(semid);
			if(elromlott >= 2){
				printf("nem romlottam el \n");
				strcpy(valasz.mtext, "Nincs baj");
				kill(getppid(), SIGRTMIN);
				
				utazasiido = rand() % 20 + 11;
				oszt_mem_ptr[0] = hanyadik;
				oszt_mem_ptr[1] = utazasiido;
				
			}else{
				printf("elromlottam \n");
				if(elromlott == 0){
					strcpy(valasz.mtext, "Fagyas");
				}else{
					strcpy(valasz.mtext, "Bananhej");
				}
				kill(getppid(), SIGRTMIN+1);
				oszt_mem_ptr[0] = hanyadik;
				oszt_mem_ptr[1] = -1;
			}
			sem_post(semid);
			
			status = msgsnd(uzenetsor, &valasz, strlen(valasz.mtext) + 1, 0);
			 
			if ( status < 0 ) 
				perror("msgsnd"); 

		
			
		
			exit(0);
		}
	}
	printf("Helyettesitesek: %i\n", mentesitok);
	printf("Atlag ido: %d \n", osszido / atoi(argv[1]));
	return 0;
}