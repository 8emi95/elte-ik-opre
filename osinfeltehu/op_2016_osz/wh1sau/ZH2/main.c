#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <string.h>

struct uzenet
{
	 long mtype;
     char mtext [1024]; 	
};

// sendig a message
int send_msg( int uzenetsor, struct uzenet uz ) 
{ 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz), 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int recieve_msg( int uzenetsor, struct uzenet* msg ) 
{ 
     int status; 
	 
     status = msgrcv(uzenetsor, &msg, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 		 
     return 0; 
} 



int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs;
	 char msg[1024];
	 struct uzenet uz;
	 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     }     
          if ( status < 0 ) 
               perror("msgctl"); 
	 child = fork();
	 if(child<0)
	 {
		 perror("Error at forking!");
		 return 1;
	 }
	 if(child==0) //child
	 {
		status = recieve_msg(uzenetsor,&uz);
		if(status<0)
			perror("Error while recieving Msg");
		int id = atoi(uz.mtext);
		printf("%d",id);
		
		
	 }
	 if(child>0) //parent
	 {		 
		printf("This is your captain speaking");
		 int train_count = 4;
		 sprintf(uz.mtext,"%d",train_count);
		 uz.mtype = 5;
		 send_msg(uzenetsor, uz );
		 wait(NULL);
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		 
	 }
}