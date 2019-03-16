#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <signal.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
};

int main (int argc, char* argv[]) { 
     pid_t kevin,thief; 
     int uzenetsor1, uzenetsor2, status; 
     key_t kulcs1,kulcs2;

     srand(time(NULL));
     
     kulcs1 = ftok(argv[0],1); 
     uzenetsor1 = msgget( kulcs1, 0600 | IPC_CREAT ); 
     if ( uzenetsor1 < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	 
	 kulcs2 = ftok(argv[0],1); 
     uzenetsor2 = msgget( kulcs2, 0600 | IPC_CREAT ); 
     if ( uzenetsor2 < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
	 kevin = fork(); 
     if ( kevin > 0 ) //parent
	 {
		thief = fork();
		if ( thief > 0 ) //parent
		{
			wait(NULL);
		}
		else if ( thief == 0 ) //thief
		{
			struct uzenet uz;
			sleep(1);
			msgrcv(uzenetsor1, &uz, sizeof (uz), 0, 0 );
			struct uzenet valasz = { 5 , "csak varj, mig a kezunk koze nem kerulsz" };
			msgsnd( uzenetsor2, &valasz, sizeof (valasz) , 0 );
			
		}
		else
		{
          perror("fork"); 
          return 1; 
	WAIT(NULL);
		}
     }
	 else if ( kevin == 0 ) //kevin
	 { 
		long toys = (rand() % 30) + 20;
		struct uzenet uz = { toys, "Szorom a jatekokat" };
		msgsnd( uzenetsor1, &uz, sizeof (uz) , 0 );
		
		sleep(2);
		struct uzenet valasz;
		msgrcv(uzenetsor2, &valasz, sizeof (valasz), 0, 0 );
		printf( "Kevin: %s\n" , valasz.mtext);

		
 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
