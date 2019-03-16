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

void handler(int signumber)
{
  
}

int main (int argc, char* argv[]) { 
     pid_t kevin,thief; 
     int uzenetsor1, uzenetsor2, status; 
     key_t kulcs1,kulcs2;
	 
	 signal(SIGINT,handler);

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
	 
	 int pipe1[2];
	 int pipe2[2];
	 
	 pipe(pipe1);
	 pipe(pipe2);
     
	 kevin = fork(); 
     if ( kevin > 0 ) //parent
	 {
		thief = fork();
		if ( thief > 0 ) //parent
		{
			int waitstatus;
			wait(&waitstatus);
		}
		else if ( thief == 0 ) //thief
		{
			sleep(1);
			struct uzenet uz;
			msgrcv(uzenetsor1, &uz, sizeof (uz), 0, 0 );
			printf( "Betoro: Hanyatt estem: %d\n" , uz.mtype);

			printf( "Betoro: Fenyegetek\n");
			struct uzenet valasz = { 5 , "csak varj, mig a kezunk koze nem kerulsz" };
			msgsnd( uzenetsor2, &valasz, sizeof (valasz) , 0 );
			
			sleep(1);
			
			kill(kevin,SIGINT);
			
			close(pipe1[1]);
			close(pipe2[0]);
			
			char pipe_message [10];
			
			read(pipe1[0], &pipe_message, sizeof(pipe_message));
			printf( "Betoro: pipe:  %s\n" , pipe_message);
		}
		else
		{
          perror("fork"); 
          return 1; 
		}
     }
	 else if ( kevin == 0 ) //kevin
	 { 
		long toys = (rand() % 30) + 20;
		struct uzenet uz = { toys, "Szorom a jatekokat" };
		printf( "Kevin: Szorom a jatekokat:%d\n" , toys);
		msgsnd( uzenetsor1, &uz, sizeof (uz) , 0 );
		
		sleep(1);
		
		struct uzenet valasz;
		msgrcv(uzenetsor2, &valasz, sizeof (valasz), 0, 0 );
		printf( "Kevin: Fenyegetes:  %s\n" , valasz.mtext);
		
		
		pause();
		
		close(pipe1[0]);
		close(pipe2[1]);
		
		char pipe_message [10] = "festek";
		
		printf( "Kevin: pipe:  %s\n" , pipe_message);
		write(pipe1[1], &pipe_message, sizeof(pipe_message));

		
 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
