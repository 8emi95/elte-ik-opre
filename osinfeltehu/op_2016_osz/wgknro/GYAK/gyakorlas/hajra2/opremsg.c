#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;
     int hanyadik;	 
}; 
struct okuzenet{
	long mtype;
	char text[1024];
};
int kuld2(int uzenetsor){
	 const struct okuzenet uz = {4, "sikeresen beert" };      
     int status;      
     status = msgsnd( uzenetsor, &uz, strlen ( uz.text ) + 1 , 0 );      
	
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}
int kuld3(int uzenetsor){
	const struct okuzenet uz = {4, "Elcsuszott egy bananhejon" };      
     int status;      
     status = msgsnd( uzenetsor, &uz, strlen ( uz.text ) + 1 , 0 );      
	
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}
int fogad2(int uzenetsor){
	struct okuzenet uz; 
     int status;      
     status = msgrcv(uzenetsor, &uz, 1024, 4, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A vegeredmeny: %s\n", uz.text );     
     return 0; 
}
void handler(int signumber){	
  printf("\n");
}
// sendig a message
int kuld( int uzenetsor,int hanyadik,int osszes ) 
{ 
     const struct uzenet uz = { 3, hanyadik };    
	 const struct uzenet uz2 = {5,osszes};
     int status; 
	 status = msgsnd( uzenetsor, &uz, sizeof ( uz.hanyadik ) , 0 );  
	 status = msgsnd( uzenetsor, &uz2, sizeof ( uz.hanyadik ) , 0 );      
	     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) { 
	 struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, sizeof(uz.hanyadik), 3, 0 );
	   	 
     if ( status < 0 ) {
		 perror("msgsnd"); 
		fflush(stdout);
		return 1;
	 }          
     else{
		 int tmp = uz.hanyadik;
		 printf( "Ez a jaratszamom:  %ld\n",tmp );  		 		 
		  fflush(stdout);
		  status = msgrcv(uzenetsor, &uz, sizeof(uz.hanyadik), 5, 0 );
		  int tmp2 = uz.hanyadik-tmp;		  
		  printf("Ennyi van még mögöttem %ld\n",tmp2);
		  fflush(stdout);
		  int random = rand()%101;		  
		  if(random <10){
			  printf("meghibasodtam\n");
			  fflush(stdout);
			  kuld3(uzenetsor);
			  kill(getppid(),SIGTERM);
			  
		  }else{
			  printf("beertem a vegallomasba\n");
			  fflush(stdout);
			  kuld2(uzenetsor);
			  kill(getppid(),SIGTERM);			  
		  }
		  return 0; 
	 }
          
     
} 

int main (int argc, char* argv[]) { 
     pid_t child;	
     int uzenetsor, status,hanyadik,osszes=atoi(argv[1]); 
     key_t kulcs;     
	 signal(SIGTERM,handler);	 
	 srand(time(NULL));	 
     kulcs = ftok(argv[0],1);      
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     }      
	 fflush(stdout);
     child = fork();	 
	 fflush(stdout);
     if ( child > 0 ) { 
		  hanyadik = rand()%(osszes+1);
		  printf("inditom a jaratot \n");
		  fflush(stdout);
          kuld( uzenetsor,hanyadik,osszes );  // Parent sends a message.		
		  //wait(NULL);
          pause(); 
		  fogad2(uzenetsor);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
	      printf("elindul a metro \n");
		  fflush(stdout);
          return fogad( uzenetsor ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
		  fflush(stdout);
          return 1; 
     } 
     
     return 0; 
} 
