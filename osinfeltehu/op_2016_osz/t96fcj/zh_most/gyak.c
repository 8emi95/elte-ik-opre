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


void handler1(int signumber){
	printf("Hopp egy uzenet! Ez a 1. gyerektol jott!\n");
  
}
void handler2(int signumber){
	printf("Hopp egy uzenet! Ez a 2. gyerektol jott!\n");
  
}

int main (int argc,char* argv[]) {
	
	
	pid_t child1;
	pid_t child2;
	pid_t parent=getpid();
	
	signal(SIGUSR1,handler1);
	signal(SIGUSR2,handler2);
	
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	
		

	
	child1= fork();
	if(child1>0) //parent process 
	{
		child2=fork();
		if(child2>0) // parent process after both child started
		{
			
			
			waitpid(child1,&status,0); 
			waitpid(child2,&status,0); 
			status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			
			
			
		}
		else if (child2==0) //child2 process (Kevin)
		{
			srand(time(NULL));
			int toys=(rand()%50)+1;
			
			struct uzenet uz = { toys, "Hajra Fradi!" }; 
			int status; 
			sleep(2);
			
			status = msgsnd( uzenetsor, &uz, sizeof ( uz.mtext ) + 1 , 0 ); 
			// a 3. param ilyen is lehet: sizeof(uz.mtext)
			// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
			if ( status < 0 ) 
			perror("msgsnd"); 
			sleep(1); 
			status = msgrcv(uzenetsor, &uz, sizeof(uz), 10, 0 ); 
			printf("Ezt mondta a betoro: %s", uz.mtext);
			
			kill(getpid(),SIGKILL);
			
		
		}
		else if (child2<0) //failed to start child2;
		{
				printf("Failed to start child process!!!\n");
				
		}
			
		
		
	}
	else 	//child1 process (betörő)
	{
		sleep(1);
		struct uzenet uz; 
		int status; 
  
		status = msgrcv(uzenetsor, &uz, sizeof(uz), 0, 0 ); 
		printf("Ennyi jatekon csusztam el: %d \n",uz.mtype);
		uz.mtype=10;
     	strcpy(uz.mtext,"Na varj csak amig elkaplak!!\n");

     //if ( status < 0 ) 
     //     perror("msgsnd"); 
 
		status = msgsnd( uzenetsor, &uz, sizeof ( uz.mtext )+1 , 0 ); 
		sleep(1);
	  
	  kill(getpid(),SIGKILL);
		
	}
			

	
	 return 0;
}