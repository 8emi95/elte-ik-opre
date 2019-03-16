#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>    
#include <sys/wait.h>
#include <wait.h> 
#include <semaphore.h> 
#include <math.h>
#include <poll.h>
#define MEMSIZE 1024
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>






struct uzenet { 
     long mtype;
     char mtext[1024]; 
}; 


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





int csoben_van_e_adat(int* fd){
	 struct pollfd lekerdezes[1];
	 lekerdezes[0].fd=*fd;
	 lekerdezes[0].events=POLLIN;
	 if (poll(lekerdezes,1,0)==1) {
		return 1;}
	else return 0;
}

int kuld( int uzenetsor, const char *uzenet_text) {
     struct uzenet temp;
	 temp.mtype=5;
	 strcpy(temp.mtext,uzenet_text);

     const struct uzenet uz = temp;
     int status; 
   
     status = msgsnd( uzenetsor, &uz, strlen( uz.mtext ) + 2 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}


struct uzenet fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     /*else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); */
     return uz; 
} 
/*
szemafor_muvelet(semid,1) -- ha lehet olvasni az osztott memóriából
szemafor_muvelet(semid,-1) -- ha nem lehet olvasni az osztott memóriából

*/


int main(int argc, char* argv[]){
	
	key_t kulcs = ftok(argv[0],1);
	
	int sh_mem_id,semid;
	semid = szemafor_letrehozas(argv[0],1); 
	sh_mem_id=shmget(kulcs,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
	int *oszottmemoria1;
	int *oszottmemoria2;
	int oszt1info=0;
	int oszt2info=0;
	
	oszottmemoria1 = (int *)shmat(sh_mem_id,NULL,0);
	oszottmemoria2 = oszottmemoria1+1;
	
	struct uzenet kerdes = {0,""};
	struct uzenet valasz1 = {0,""};
	struct uzenet valasz2 = {0,""};
	
	int megert1 =0;
	int megert2 =0;
	
	int veletlen1=0;
	int veletlen2=0;
	int valasz =0;
	int fd_gyerek1[2]; 
	int fd_gyerek2[2]; 
	time_t t;
	srand((unsigned) time(&t));
	int status;
	pid_t child1;
	pid_t child2; 
	pid_t waitpid; 
	
	int uzenetsor;
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	 if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	 
	 if( pipe(fd_gyerek1) == -1 ){ 
		perror("Baj a csovel"); 
		exit(EXIT_FAILURE);}
	
	
	 if( pipe(fd_gyerek2) == -1 ){ 
		perror("Baj a csovel"); 
		exit(EXIT_FAILURE);}
	
	fflush(stdout);
	child1 = fork();
	if(child1==-1){
		perror("Fork hiba");
        exit(EXIT_FAILURE);
	}
	
	
	if(child1>0){
		child2 = fork();
	}
	
	if(child1>0 && child2>0){
		kuld(uzenetsor,"Mennyi az annyi?\n");
		kuld(uzenetsor,"Mennyi az annyi?\n");
		 while (1)
		{	if(!((waitpid= wait(&status)) > 0))
			break;
		}
		read(fd_gyerek1[0], &valasz, sizeof(valasz));
		printf("Az elso gyerek valasza: %d\n",valasz);
		fflush(stdout);
		read(fd_gyerek2[0], &valasz, sizeof(valasz));
		printf("A masodik gyerek valasza: %d\n",valasz);
		fflush(stdout);
		if(*oszottmemoria2!=0){
			printf("A masodik gyerek nem ertette meg: %d\n",*oszottmemoria2);
			fflush(stdout);
		}
		if(*oszottmemoria1!=0){
			printf("Az elso gyerek nem ertette meg: %d\n",*oszottmemoria1);
			fflush(stdout);
		}
		 shmdt(oszottmemoria1);
		 shmdt(oszottmemoria2);
		 szemafor_torles(semid);
		 shmctl(sh_mem_id,IPC_RMID,NULL);
		
	}
	
	if(child2==0){
		while(1){
			valasz2 = fogad(uzenetsor);
			if(strcmp(valasz2.mtext,"Mennyi az annyi?\n")==0){
				break;
			}
		}
		sleep(1);
		printf("A kerdest megkapta a 2. gyerek, ami: %s",valasz2.mtext);
		fflush(stdout);
		veletlen2 = (rand() % 4+1);
		write(fd_gyerek2[1], &veletlen2, sizeof(veletlen2));
		if(megert2==0){
			szemafor_muvelet(semid,-1);
			*oszottmemoria2=2;
			szemafor_muvelet(semid,1);
			++megert2;
		}
		else 
			--megert2;
		
		
	}
	
	if(child1==0){
		while(1){
			valasz1 = fogad(uzenetsor);
			if(strcmp(valasz1.mtext,"Mennyi az annyi?\n")==0){
				break;
			}
		}
		sleep(1);
		printf("A kerdest megkapta az 1. gyerek, ami: %s",valasz1.mtext);
		fflush(stdout);
		veletlen1 = (rand() % 4+1);
		write(fd_gyerek1[1], &veletlen1, sizeof(veletlen1));
		if(megert1==0){
			szemafor_muvelet(semid,-1);
			*oszottmemoria1=1;
			szemafor_muvelet(semid,1);
			++megert1;
		}
		else 
			--megert1;

	}
	
	
	
	
	
	
	
	/*
	if (child1==0){
		kuld(uzenetsor,"Keszen all a child1\n");
		int temp=0;
		while(1){printf("Child1: Varok\n");
				 fflush(stdout);
			read(fd[0], &temp, sizeof(temp));
			if(temp!=0){
				break;
			}
		}
		int j=2;
		printf("Gyerek1: Fogadtam az adatokat\n");
		fflush(stdout);
		printf("Az 1. jelentkezo azonositoszama: %d\n",temp);
		read(fd[0], &temp, sizeof(temp));
		while(temp!=0){
			printf("Child1: irok\n");
		    fflush(stdout);
			printf("A %d . jelentkezo azonositoszama: %d\n",j,temp);
			read(fd[0], &temp, sizeof(temp));
			++j;
		}
		kuld(uzenetsor,"Gyerek1: Adatkiiras kesz!\n");
			
		}
	
	
	if(child1>0){
	   child2 = fork();
	    if(child2==-1){
		perror("Fork hiba");
        exit(EXIT_FAILURE);}
	   if(child2>0){
		   temp = fogad(uzenetsor);
		   while(1){printf("Varok\n");
				    fflush(stdout);
			   if (strcmp(temp.mtext,"Keszen all a child1\n")==0 && strcmp(temp.mtext,"Keszen all a child2\n")==0){
				   break;}  
			   temp = fogad(uzenetsor);  
		   }
		printf("Vartam\n");
	    fflush(stdout);
		int i;
		for (i=0; i<hanyszavazo; ++i){
		szavazok[i]=(rand() % 100);
		write(fd[1], &szavazok[i], sizeof(szavazok[i]));
			}
		temp=fogad(uzenetsor);
		while(1){
			printf("Child2: varok\n");
		    fflush(stdout);
			if (strcmp(temp.mtext,"Gyerek1: Adatkiiras kesz!\n")==0){
				break;}
		}
	   }
	if (child2==0){
		printf("Child2 belepve\n");
		fflush(stdout);
	   kuld(uzenetsor,"Keszen all a child2\n");
		 }
		
		
		
	}*/
return 0;
}