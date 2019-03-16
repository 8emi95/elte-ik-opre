#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/ipc.h>


struct message{
	int sorszam;
	int ossz;
};

	int msgqueue;
	


void child_proc(int id)
{

  int fail = (rand() % 10);
  if(rand() % 2 == 0){
	char reason[100] = "Elcsuszott egy bananhejon.";
  } else {
	  char reason[100] = "Lefagyott a zold telben.";
  }
  // kiiras
  }
  
int main(int argc, char* argv[])
{
	printf("Budapest Metro Funs tasrsasag...\n");
	int arg = atoi((argv[1]));
	printf("%d \n",arg);
	struct message msg;
	msg.sorszam = (rand() % (arg + 1)) - 1;
	msg.ossz = arg;
	printf("%d %d",msg.sorszam, msg.ossz);
	
	pid_t child;
	key_t key;
	key = ftok(argv[0],1); 
	child = fork();
	msgqueue = msgget( key, 0700 | IPC_CREAT ); 
	
	if ( child > 0 ) { 
          const struct message msg = { (rand() % (arg + 1)) - 1, arg }; 
     int status; 
     
     status = msgsnd( msgqueue, & msg, 2 , 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd");   // A szülőfolyamat küldi az üzenetet. 
          wait( NULL ); 
          status = msgctl( msgqueue, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
         // return fogad( msgqueue ); 
          // A gyerekfolyamat fogadja az üzenetet. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
	return 0;
}