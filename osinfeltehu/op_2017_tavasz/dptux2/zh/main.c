#include <stdio.h>
#include <stdlib.h>//exit
#include <sys/msg.h> 
#include <sys/types.h>
#include <signal.h>
#include <string.h> 
#include <unistd.h> //fork,pipe

void handler(int signumber){
}

struct mymsg {
    long int mtype;       /* message type */
	pid_t mpid;    		/* message text */
};

int sending(int msgbuffer, pid_t pt,long int t){
	
	int status;
	struct mymsg message;
	message.mpid = pt;
	message.mtype = t; 
	
	status = msgsnd( msgbuffer,&message,sizeof(message.mpid) , 0); 
	
	if ( status < 0 ) 
          perror("Error: Message could not be sent!\n"); 
     return 0; 
};

int receiving(int msgbuffer,pid_t* pt,long int t){
	 struct mymsg message; 
     	int status; 
     
     status = msgrcv(msgbuffer, &message, 1024, t, 0 ); 
     
     if ( status < 0 ) 
          perror("Error: Message could not be received!\n");  
     else
          *pt = message.mpid;
	  
     return 0; 
};


int main(int argc, char *argv[]){
	
	srand(time(NULL));
	char * termeknev = argv[1];
	int ar = atoi(argv[2]);
	
	//Block procmask
	sigset_t block_mask;
	sigemptyset (&block_mask);
	sigaddset (&block_mask, SIGUSR1);
	sigprocmask (SIG_SETMASK, &block_mask, NULL);
	//Critical procmask
	sigset_t critical_mask;
	sigfillset(&critical_mask);
	sigdelset(&critical_mask,SIGUSR1);
	
	//Handler setup
	signal(SIGUSR1,handler);
	
	//massage
	int status; 
    key_t key = ftok(argv[0],1);	
	int msgbuffer = msgget( key, 0600 | IPC_CREAT );//?? 
     if ( msgbuffer < 0 ) { 
          perror("Error: Message buffer could not be created!\n"); 
          return 1; 
     } 
	
	
	//pipe
	int pipe1[2];
	
	if(pipe(pipe1) == -1){
		perror("The pipe calling was not successful\n");
        exit(1);
	}

	//fork
	 pid_t child1 = fork();
	
	if(child1 < 0){
		perror("The fork calling was not successful\n");
		exit(1);
	}
	else if(child1 > 0){
	//parent
		pid_t child2 = fork();
		if(child2 < 0){
			perror("The fork calling was not successful\n");
			exit(1);
		}
		else if(child2 > 0){
			//parent
			sending(msgbuffer,child2,1);
			close(pipe1[0]);
			close(pipe1[1]);
			wait();
			wait();
			printf("Vallalat vege!\n");
		}
		else{
			//vasarlo
			sigsuspend(&critical_mask);
			pid_t ugynok;
			read(pipe1[0],&ugynok,sizeof(ugynok));
			kill(ugynok,SIGUSR1);
			printf("Telefon felveve\n");
			int ujar = ar;
			int i = 1;
			
			do{
				read(pipe1[0],&ujar,sizeof(ujar));
				printf("Ajanlat: %i\n",ujar);
				i = rand() % 2;
				if(i)
					printf("Vasarlo elutasit\n");
				else
					printf("Vasarlo elfogad\n");
				write(pipe1[1],&i,sizeof(i));
				kill(ugynok,SIGUSR1);
				sigsuspend(&critical_mask);
			}while(i && (ujar*2 >= ar));
			printf("Vegso ar :%i\n",ujar);
			close(pipe1[0]);
			close(pipe1[1]);
			printf("Vasarlo vege!\n");
		}
		
	}	
	else{
		//ugynok	
		
		pid_t vasarlo;
		receiving(msgbuffer,&vasarlo,1);
		printf("Vasarlo adatok megerkeztek\n");
		pid_t t  = getpid();
		write(pipe1[1],&t,sizeof(t));
		printf("Vasarlo felhivasa\n");
		kill(vasarlo,SIGUSR1);
		sigsuspend(&critical_mask);
		int i = 1;
		int ujar = ar;
		do{
			write(pipe1[1],&ujar,sizeof(ujar));
			sigsuspend(&critical_mask);
			read(pipe1[0],&i,sizeof(i));
			kill(vasarlo,SIGUSR1);
			if(i && (ujar*2 >= ar))
				ujar = ujar - 10;
		}while(i && (ujar*2 >= ar));
		close(pipe1[0]);
		close(pipe1[1]);
		printf("Ugynok vege\n");
		
	}
	msgctl( msgbuffer, IPC_RMID, NULL );
	
	return 0;
};