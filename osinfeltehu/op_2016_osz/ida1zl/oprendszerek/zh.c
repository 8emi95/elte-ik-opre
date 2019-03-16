#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     /*long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; */
	 int r;
	 int ossz;
}; 

// sendig a message
int kuld( int uzenetsor, int hanyadik, int osszes) 
{ 
     const struct uzenet uz = {hanyadik, osszes }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.r), 0 ); 
	 printf("beirtam %d %d\n", uz.r, uz.ossz);
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int kuld2(int uzenetsor)
{
	char s[25] = "elcsusztam egy bananhejon";
	int status; 
     
     status = msgsnd( uzenetsor, &s, sizeof(s) , 0 ); 
	 printf("beirtam %s", s);
	    if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, sizeof(&uz), 0, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
		  
          printf( "A %d. szerelveny vagyok. Meg %d van hatra\n", uz.r, uz.ossz - uz.r ); 
     return 0; 
} 

int fogad2(int uzenetsor)
{
		char sz[25];
		int status;
		status = msgrcv(uzenetsor, &sz, sizeof(sz), sizeof(sz), 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet szovege:  %s\n", sz); 
     return 0; 
	
}



 void handler(int signumber){
  printf("szignal jott, hogy bevontattak\n",signumber);
}


int main (int argc, char* argv[]) { 
	pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     int max;
	 printf("Összesen hany szerelveny lesz? ");
	 scanf("%d", &max);
     // msgget needs a key, amelyet az ftok generál 
     //. 
	
	 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	 
	 signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);


	 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
		 int random=rand()%101;
     child = fork(); 
     if ( child > 0 )	//parent
	 {
		 //random
		srand(time(NULL)); 
		int rn=rand()%max + 1;
		printf("random szam:%d\n", rn);
		
		  kuld( uzenetsor, rn, max );  // Parent sends a message. 
		  
          wait( NULL ); 
		  
		  //int f = fogad2(uzenetsor);
		   kill(child, SIGUSR1);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
	 }
	 else	//child
	 {
		 printf("gyerek elindul\n");
		 printf("%d\n", random);
		 if(random <= 90)
		 {
			 printf("siker\n");
		 }
		 else
		 {
			 printf("hiba\n");
			 kill(getppid(),SIGUSR1);
			 //uld2(uzenetsor);
		 }
		 
        return fogad( uzenetsor );  
	 }
     return 0; 
} 
