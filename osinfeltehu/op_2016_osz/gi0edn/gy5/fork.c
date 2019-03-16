#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 


struct message { 
     long mtype;
     char mtext[1024]; 
};

 int msgqueue; 

int main()
{
	 int status;
	 key_t key;
	key = ftok(argv[0], 1);
	msgqueue = msgget(key, 0600 | IPC_CREAT);
	 if (msgqueue < 0)
  {
    perror("msgget");
    exit(1);
  }

  
	 
	 pid_t  child=fork();
	 
	 //hiba
	 if (child<0){
		 perror("The fork calling was not succesful\n"); 
		 exit(1);
	} 
	 if (child>0)
	 {//szülő
		waitpid(child,&status,0);
		printf("Szulo \n");
	 
	 }
	 else
	 {//gyerek
		printf("Gyerek \n");
	 }
	 return 0;
}