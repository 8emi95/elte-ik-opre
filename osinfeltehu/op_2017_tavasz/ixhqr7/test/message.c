#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct messg { 
     long mtype;//this is a free value e.g for the address of the message
     char mtext [ 1024 ]; //this is the message itself
}; 

void handler(int signumber){
    printf("Signal with number %i has arrived\n",signumber);
}

void signalSender(){
    kill(getppid(),SIGTERM);
}

int send( int mqueue ) 
{ 
     const struct messg m = {3 ,"51 , 21, 25" }; 
     int status; 
     
     status = msgsnd( mqueue, &m, strlen ( m.mtext ) + 1 , 0 ); 
	// a 3rd parameter may be : sizeof(m.mtext)
     	// a 4th parameter may be  IPC_NOWAIT, equivalent with  0-val 
     if ( status < 0 ) 
          perror("msgsnd error");      
     
     return 0; 
} 
     
int receive( int mqueue ) 
{ 
     struct messg m; 
     int status; 
     // the last parameter is the number of the message
	// if it is 0, then we read the first one
	// if  >0 (e.g. 5), then message type 5 will be read
	 
     status = msgrcv(mqueue, &m, 1024, 3, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd error"); 
     else
         // printf( "The coordinates are : %ld, \n text is:  %s\n", m.mtype, m.mtext ); 
         printf("\n The coordinates are: %s \n", m.mtext);     
     
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child;
     int messg, status2;
     key_t key;
             
     key = ftok(argv[0],1);
                 
     int status;
     signal(SIGTERM, handler);
     int fd[2];
     pipe(fd);
   
               
     // msgget needs a key, created by  ftok  
     //. 
     //printf ("The key: %d\n",key);
     messg = msgget( key, 0600 | IPC_CREAT ); 
     if ( messg < 0 ) { 
          perror("msgget error"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          send( messg );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( messg, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl error"); 
          
          int hustonCoors;
          int nOfOccurences;
          
          srand(time(NULL));
          
          nOfOccurences = rand() % 10 + 1;
          hustonCoors = rand () % 900 + 100;
          
          printf("The parent's generated coordinates is  : %d\n", hustonCoors);
          printf("The number of occurences will be  : %d\n", nOfOccurences );
          printf("Signal from Robot has been received\n");
          write(fd[1], &hustonCoors, sizeof(hustonCoors));
          write(fd[1], &nOfOccurences, sizeof(nOfOccurences ));          
          
          
          //return 0;          
     } else if ( child == 0 ) { 
          receive( messg );
          int coorDinates;
          
          read(fd[0], &coorDinates, sizeof(coorDinates));
          printf("Coordinates sent by Huston are:  %d\n", coorDinates);
           
     } else { 
          perror("fork error"); 
          return 1; 
     }     
     return 0; 
}