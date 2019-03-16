#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct messg { 
     
	long mtype;
     
	int check; 

};

int send( int mqueue, int check ) 
{ 
     

	const struct messg m = { 5, check }; 
     
	int status; 
     
     
	status = msgsnd( mqueue, &m, sizeof(check), 0 ); 
	
	
    
	 if ( status < 0 ) 
          
		perror("msgsnd error"); 
     
	return 0; 

} 

int receive( int mqueue ) 
{ 
     
	struct messg m; 
     
	int status; 
 
	 
     
	
	status = msgrcv(mqueue, &m, sizeof(int), 5, 0 ); 
     
    
	 if ( status < 0 ) 
          
		perror("msgrcv error\n"); 
     
	 else
{ 
		if(m.check==2){         
			printf( "The code of the message is : %s!\n", "number should be bigger"); 
 
		}else if(m.check==0){
			printf( "The code of the message is : %s!\n", "number should be smaller"); 
 
		}else if(m.check==1){
			printf( "The code of the message is : %s!\n", "number is correct"); 
 
		}   
	} 
	return 0; 

} 

void handler(int sig){
	printf("Child signaling parent that it is ready to play!\n");
	signal(sig, handler);
}

int main(int argc, char *argv[]){

	signal(SIGUSR1, handler);
	srand(time(NULL));

	pid_t child;
	int pipe1[2];
	
	int messg, status; 
     
	key_t key; 
	
	int c;
	int guessed=0;
	int range=100;

	key = ftok(argv[0],1); 
     
     
	messg = msgget( key, 0600 | IPC_CREAT ); 
 
    
	if ( messg < 0 ) { 
          
		perror("msgget error"); 
          
		return 1; 
     
	} 

	if(pipe(pipe1)==-1){
		perror("Error in creating piepe!\n");
		exit(1);
	}

	int number1=rand()%100+1;
	
	printf("First number is: %d!\n", number1);

	child=fork();

	if(child<0){
		perror("Error creating fork!\n");
		exit(1);
	}

	if(child==0){
		kill(getppid(), SIGUSR1);// sending signal to parent
		close(pipe1[0]); //close reading end
		int guess;
		int i=0;
		while(guessed<1){
			if(c==2){
				guess=rand()%range + (100-range+1);
				
			}else if(c==0){
				//guess=rand()%range + (100-range+1);
				guess=rand()%range+1;
			}
			write(pipe1[1], &guess, sizeof(guess));
		}
		close(pipe1[1]); //close writing end
		return receive(messg);
		exit(0);
		
	}else{
		pause(); //waiting for signal from child
		close(pipe1[1]); //close the write
		
		//int c;

		int guess;
		while(read(pipe1[0], &guess, sizeof(guess))){
		printf("Child guessed: %d!\n", guess);
		
		if(guess<number1){
			c=2;
			range=guess;
		}else if(guess >number1){
			c=0;
			range=guess;
		}else if(guess==number1){
			c=1;
			guessed=1;
		}
		}

		close(pipe1[0]); //closing reading
		
		send(messg, c);
		
		wait();  //waiting for child to finish	
		
		status = msgctl( messg, IPC_RMID, NULL); 
         
		if ( status < 0 ) 
               
			perror("msgctl error"); 
         
		return 0; 
		
	}

	return 0;
}
