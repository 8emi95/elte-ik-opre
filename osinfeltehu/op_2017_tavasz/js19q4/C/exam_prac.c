#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 


struct messg { 
     long mtype;//this is a free value e.g for the address of the message
     int mtext [ 45 ]; //this is the message itself
}; 

int send(struct messg * m, int mqueue ) 
{ 
     
     int status; 
     
     status = msgsnd( mqueue, &m, sizeof ( m.mtext ) , 0 ); 
	// a 3rd parameter may be : sizeof(m.mtext)
     	// a 4th parameter may be  IPC_NOWAIT, equivalent with  0-val 
     if ( status < 0 ) 
          perror("msgsnd error"); 
     return 0; 
} 

// receiving a message. 
int receive( int mqueue ) 
{ 
     struct messg m; 
     int status; 
     // the last parameter is the number of the message
	// if it is 0, then we read the first one
	// if  >0 (e.g. 5), then message type 5 will be read
	 
     status = msgrcv(mqueue, &m, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd error"); 
     else
          printf( "The code of the message is : %ld, text is:  %s\n", m.mtype, m.mtext ); 
     return 0; 
} 


int condition = 1;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  condition = 0;
}



int main(int argc, char const *argv[])
{
	
	signal(SIGTERM, handler);
	pid_t child, child1;

	srand(time(NULL));
	//printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
	
	int r1,r2,r3;
	
	int fd;
	int num_voters = atoi(argv[1]);
	int voters[100];
	int read_voters[100];
	int myPipe[2];
	char named_pipe[1024]="";	
	int status, messg;
	key_t key;
	
	key = ftok(argv[0],1); 
    printf ("The key: %d\n",key);
    messg = msgget( key, 0600 | IPC_CREAT ); 
    if ( messg < 0 ) { 
         perror("msgget error"); 
         return 1; 
    } 
	 
	if (fid==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
	
	if (pipe(myPipe) == -1) 
	{
               perror("Opening error!");
               exit(EXIT_FAILURE);
    	}

	if( (child=fork()) < 0){
		perror("Fork first child error");
	} 
	else if(child > 0){
		printf("Second child is created !!\n");
		if((child1 = fork()) < 0){
			perror("Fork second child error");
		}
	}
	
	if(child == 0){ //first child
		sleep(2);
		//printf("I am first child process and my pid %u and my parent pid is %u\n", getpid(), getppid());
		printf("First child is sending signal to parent\n");
		//pause();
		kill(getppid(), SIGTERM);
		sleep(2);
		//printf("FIrst child finished\n");
		close(myPipe[1]);
		read(myPipe[0],read_voters,sizeof(read_voters));
		int i;
		
		int n = num_voters;
		printf("Number of voters %d\n", n);
		
		for(i = 0; i < n;i++){
			printf("Child read the message: %d\n",read_voters[i]);
		}
		
		kill(getppid(), SIGTERM);
	    printf("\n");
        close(myPipe[0]); // f
		
		sleep(2);
		fd=open("fifo.ftc",O_WRONLY);
        write(fd,"20% they are not good!\n",23);
        close(fd);
		sleep(2);

		
	}	
	else if(child1 == 0){ //second child
			//pause();
			sleep(4);
			close(myPipe[0]);
			close(myPipe[1]);
			//printf("I am second child2 process my pid is %u and my parent pid is %u\n", getpid(), getppid());
			printf("Second child is sending signal to parent\n");
			kill(getppid(), SIGTERM);
			sleep(4);
			//printf("Second child finished\n");
			sleep(4);
			fd=open("fifo.ftc",O_RDONLY);
			read(fd,named_pipe,sizeof(named_pipe));
			printf("I read from the pipe:  %s \n",named_pipe);
			close(fd);
			// remove fifo.ftc
			unlink("fifo.ftc");
			kill(getppid(), SIGTERM);
			sleep(1);
			
			int rdm = rand() % 6;
			int send_m [45];
			send_m[0] = rdm;
			const struct messg m = { send_m, 5}; 
			send( &m, messg );
			status = msgctl( messg, IPC_RMID, NULL ); 
			kill(getppid(), SIGTERM);
			
			
			
			
	}
	
	else{
	 
		close(myPipe[0]);
		printf("I am the parent\n");
		
		pause();
		printf("Parent notices first child is ready\n");
		int status = 0;
	
		pause();
		printf("Parent notices second child is ready\n");
	
		printf("Parent got ready\n\n");

		//wait(&status);
		printf("Enter number of voters\n");
		//scanf(" %d", &num_voters);
		int i = 0;	
		//write(myPipe[1], num_voters, sizeof (int));		
		for(i; i < num_voters; i++){
			r1 = rand();
			voters[i] = r1;
			write(myPipe[1], voters, sizeof voters / sizeof *voters);
			printf("Parent wrote %d to unnamed pipe \n", r1);
			close(myPipe[1]);
		} 
		pause();

		printf("Parent called children again\n");
		pause();
		printf("Who can vote??");
		pause();
		receive( messg );
		

		
		
	}
		

	//printf("Out of loop\n");




	return 0;
}