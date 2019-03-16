#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

int ossz;

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){}

int kuld( int uzenetsor, int maxszerelveny) { 
	srand(time(NULL));
	int rndm = rand() % 100;
	char randomNumber[5] = "";
	
	sprintf(randomNumber, "%d", rndm);
	
	 struct uzenet uz = { 5, randomNumber[0]}; 
	 int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

int kuld2(int uzenetsor) { 
	 struct uzenet uz = { 5, "Meghibasodtam."}; 
	 int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

int kuld3(int uzenetsor) { 
	 struct uzenet uz = { 5, "Minden rendben volt."}; 
	 int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int main (int argc, char* argv[]) {
     pid_t child;
	 
     int uzenetsor, status; 
     key_t kulcs; 
     kulcs = ftok(argv[0],1); 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
	 signal(SIGTERM, handler);
	 
     child = fork(); 
	 
     if ( child > 0 ) {
		  kuld( uzenetsor, atoi(argv[0]));
		  pause();
		
		//Meghibasodas olvasasa
		struct uzenet uz; 
		int status; 
		status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 

		if ( status < 0 ){
			perror("msgsnd");	 
		}

		else{
			printf( "A szerelveny jelzese: %s\n", uz.mtext ); 
		}
		  
          wait( NULL ); 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } 
	 
	 else if (child == 0) {
		srand(time(NULL));
		int rndm = rand() % 100;
		char randomNumber[5] = "";
		sprintf(randomNumber, "%d", rndm);

		 struct uzenet uz; 
		 int status; 
		 status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
		 
		 if ( status < 0 ){
			perror("msgsnd");	 
		 }
	 
		 else{
			 printf( "Ma a %s. vagyok.\n", uz.mtext ); 
		}
		
		if (rndm >= 50){
			ossz = atoi(argv[1])-atoi(uz.mtext);
			printf("Meghibasodtam.\n%s. szerelveny vagyok.\n%d szerelveny van mogottem.\n",uz.mtext,ossz);
			kuld2(uzenetsor);
		}
		else{
			kuld3(uzenetsor);
		}
		
		kill(getppid(), SIGTERM);
     }
	 
	 else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
