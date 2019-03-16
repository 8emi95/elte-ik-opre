#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <string.h>

struct uzenet
{
	 long mtype;
     char mtext [ 1024 ]; 	
};

// sendig a message
int send_msg( int uzenetsor, char* msg ) 
{ 
     const struct uzenet uz = {5 , *msg }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
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
     uzenetsor = msgget( kulcs, 7700 | IPC_RMID ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
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
		 int train_count = 1;
		 sprintf(msg,"%d",train_count);
		 send_msg(uzenetsor, msg );
		 
	 }
	 
}