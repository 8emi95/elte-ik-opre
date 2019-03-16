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



//Globals
int pipefd[2];
int counter = 0;
int resultOfGuess = 4;
int gameCounter = 0;

struct messg { 
     long mtype;//this is a free value e.g for the address of the message
     int result; //this is the message itself
}; 


//Prototypes
void handler(int signalNumber);
int send(int mqueue, int toSend);
int recieve(int mqueue);

int main (int argc, char ** argv){

	int status;
	int messg;
	srand(getpid());
	pid_t child; 
	signal(SIGTERM,handler);

	key_t key;

	//Shared Mem
	int sh_mem_id;
	int *sharedMemory;

	// sh_mem_id = shmget(NULL, 1024, IPC_CREAT|S_IRUSR|S_IWUSR|IPC_PRIVATE);
	sh_mem_id=shmget(key,1024,IPC_CREAT|S_IRUSR|S_IWUSR);
	sharedMemory = shmat(sh_mem_id, NULL, 0);
	 

	key = ftok(argv[0], 1);


	messg = msgget(key, 0600 | IPC_CREAT);
	if ( messg < 0 ) { 
      perror("msgget error"); 
      return 1; 
   } 

	if (pipe(pipefd) == -1) 
 	{
   	perror("Error in pipe\n");
   	exit(EXIT_FAILURE);
 	}

	int randomNumber;
	randomNumber = rand() % 100+1; 
	printf("I thought of the number: %d\n",randomNumber );


	child = fork();

	if (child > 0) //***Parent***
	{
		int guessOfChild;

		close(pipefd[1]);

		resultOfGuess = 4;

		printf("I got the signal\n");
		while (resultOfGuess != 0){
		read(pipefd[0], &guessOfChild, sizeof(guessOfChild));
		printf("Parent: the child's guess is %d\n",guessOfChild);
	

		//3 Cases   
		if (guessOfChild == randomNumber)
		{
			resultOfGuess = 0;
		}
		if (guessOfChild < randomNumber)
		{
			resultOfGuess = 1;
		}
		if (guessOfChild > randomNumber)
		{
			resultOfGuess = 2;
		}

		send(messg, resultOfGuess);
		wait(NULL);
		}
		// sharedMemory[0]++;
		printf("Final Counter: %d\n",gameCounter );
		status = msgctl( messg, IPC_RMID, NULL ); //Mark to be removed

	} else {  //***Child***
		close(pipefd[0]);

		int roundResult = 4;
		int childGuess;
		printf("I am the child. \n");
		while(roundResult != 0){
		kill(getppid(), SIGTERM);
		childGuess = rand()%100+1; 
		printf("Child: I thought of %d\n", childGuess);
		write(pipefd[1], &childGuess, sizeof(childGuess)); //Child sends 
		gameCounter++;
		}
		close(pipefd[0]);

		struct messg m2;

		status = msgrcv(messg, &m2, sizeof(m2.result), 5, 0);
		printf("The result of the match is : %d\n",m2.result );

		// printf("Hello: %d\n", sharedMemory[0]);

		
		
	}

}

void handler(int signalNumber){
	counter++;
}

int send(int mqueue, int toSend){
	int status; 
	int result = toSend;
	const struct messg m = {5, result};
	status = msgsnd(mqueue, &m, sizeof(m.result), 0);

	if (status < 0)
	{
		perror("msgsnd error"); 
	}

	return 0;
}

int recieve(int mqueue){
	
	struct messg m;
	int status; 

	status = msgrcv(mqueue, &m, sizeof(m.result), 5, 0);
	return 0;
	
}
