#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>  
#include <errno.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor, char* uzenet, int pid) 
{
	char buffer[50];
	strcpy(buffer,uzenet);
	
    //const struct uzenet uz = { pid, buffer }; 
	struct uzenet* uz = (struct uzenet *)malloc(sizeof(uzenet));
	strcpy(uz->mtext, buffer);
	uz->mtype = pid;
	
    int status; 
     
    status = msgsnd( uzenetsor, uz, sizeof(uz->mtext) , 0 ); 
	printf("Uzenet elkuldve: %s \n", uz->mtext);
	
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
int fogad( int uzenetsor, int pid ) 
{ 
    struct uzenet uz; 
    int status; 
    
    status = msgrcv(uzenetsor, &uz, 1024, pid, 0 ); 
     
    if ( status < 0 ) 
        perror("msgsnd"); 
    else
        printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
    return 0; 
} 

void handler(int signumber){
	printf("A %i szamu szignal megerkezett\n",signumber);
}

int main(){
	/*
	key_t osztmem_kulcs;
    int oszt_mem_id;
	char *s;
	osztmem_kulcs=ftok(argv[0],1);
	oszt_mem_id=shmget(osztmem_kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
	s = shmat(oszt_mem_id,NULL,0);
	*/
	
	int uzenetsor;
	key_t kulcs; 
	
	//kulcs = ftok(argv[0],1); 
	//printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget( /*vagy kulcs*/ IPC_PRIVATE , 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
	    perror("msgget"); 
	    return 1; 
	} 
	
	signal(SIGUSR1, handler);
	
	int pipefd[2]; 
	pid_t pid;
	char sz[100];  
	if (pipe(pipefd) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
    }
	
	int status;
	
	pid_t child=fork(); 
	
	if (child<0){
		error("Fork hiba");
        exit(EXIT_FAILURE);
	}
	
	if (child>0){
		
		//write(pipefd[1], "Hajra Fradi! \n",13);
		
		char uz[20] = "Hajra fradi";		  
		kuld( uzenetsor , uz, child);  
		fogad(uzenetsor, getpid());
		
		//waitpid(child,&status,0); ha 3 folyamat van, ez utan + wait();
		wait(&status);
		
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
        if ( status < 0 ){
			perror("msgctl"); 
		}   
		/*
		char buffer[] = "Hajra Fradi! \n";                   
        // beirunk a memoriaba 
        strcpy(s,buffer);
        // elengedjuk az osztott memoriat
        shmdt(s);	   
	    shmctl(oszt_mem_id,IPC_RMID,NULL);
	   */
	}
	else{
		read(pipefd[0],sz,sizeof(sz)); 
		fogad( uzenetsor, getpid() ); 
	    char uz[20] = "Hajra vasas";	
		kuld(uzenetsor, uz, getppid());
		
		//shmdt(s);
	}
	return 0;
}