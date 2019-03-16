#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){
}

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof ( uz.mtext ) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
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
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main () 
{ 
	signal(SIGUSR1,handler);
	pid_t parentID = getpid();
	pid_t pid;
	pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok("./",1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     int i;
	for(i=0;i<2;++i){
		if(parentID == getpid()){
			if((pid=fork())<0) {
				printf("forking error (%d)",i);
				exit(1);
			}
		}
	}
	if(pid == 0) { // child

		printf("Gyerek el es virul (id: %d)\n",getpid());
		sleep(3);
   		kill(getppid(),SIGUSR1);
		fogad( uzenetsor ); 
		sleep(2);
		printf("Gyerek voltam (id: %d)\n",getpid());
		
	}
	else { // parent
		printf("Szulo itt van (id: %d)\n",getpid());
		for (i=0;i<2; ++i){
			pause(); 
		}
		
		printf("Elkeszultek a gyerekek!\n",SIGUSR1);
		
		for (i=0; i<2; ++i){
			 kuld( uzenetsor ); 
		}
		 
		//wait for the children 
		for (i=0; i<2; ++i){
			int st;
			wait(&st);
		}
		
		printf("Szulo vegzett (%d)\n",getpid());
	}
	
	return 0;
}
