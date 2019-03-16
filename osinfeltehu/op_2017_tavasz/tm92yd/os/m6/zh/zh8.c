//Mikus Márk - Oprendszerek ZH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <wait.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

/*
* uzenet
*/
struct message
{ 
	long mtype;			//tipus ha kell
	char mtext [ 576 ];  //uzenet tartalma
};

/**
 * Uzenetet kuldése
 **/
int send( int queue ) {
	const char *question = "\nHanyadik lesz Magyarorszag az eb csoportjaban?\n";
	struct message msg = { 5, "Uzenet indul..." };
	int status;
	
	strcpy(msg.mtext, question);
	status = msgsnd( queue, &msg, strlen ( msg.mtext ) + 1 , 0 ); 
	if ( status < 0 ) { 
		perror("msgsnd");
	}
	
	return 0; 
}

/**
* Uzenet fogad
**/
int fogad( int queue ) {
	struct message msg; 
	int status; 
	status = msgrcv(queue, &msg, 576, 5, 0 ); 
     
	if (status < 0) {
		perror("msgsnd");
	} else {
		printf( "A kerdes:  %s", msg.mtext ); 
	}
	
	return 0; 
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int queue, status;
	key_t kulcs;
	int pipefd1[2];
	int pipefd2[2];
	if (pipe(pipefd1) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	if (pipe(pipefd2) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	kulcs = ftok(argv[0],1); 
	queue = msgget(kulcs, 0600 | IPC_CREAT); 
	if (queue < 0) { 
		perror("msgget"); 
		return 1; 
	}
	

	pid_t child = fork();
	if (child > 0) {
		pid_t child2 = fork();
		
		//2. gyerek
		if(child2 == 0) {
			printf("2. diak: kerdes adott, gondolkozom\n");
			fogad(queue);
			sleep(2);
			
			int v = rand()%4+1;
			v = rand()%4+1;
			close(pipefd2[0]);
			switch(v) {
				case 1:
					write(pipefd2[1], "1",2);
					break;
				case 2:
					write(pipefd2[1], "2",2);
					break;
				case 3:
					write(pipefd2[1], "3",2);
					break;
				case 4:
					write(pipefd2[1], "4",2);
					break;
			}
			close(pipefd2[1]);
			fflush(NULL); 
		} else { //szülö
			int status;
			printf("A tanar vagyok, kerdesek kuldese...\n");
			
			send(queue);
			send(queue);
			wait(&status);
			//close gyerekek
			close(pipefd1[1]);  
			close(pipefd2[1]);
			
			char v1[2];
			char v2[2];
			
            read(pipefd1[0],v1,sizeof(v1)); 
			read(pipefd2[0],v2,sizeof(v2)); 
			
			printf("Az elso diak valasza %s\n", v1);
			printf("A masodik diak valasza %s\n",v2);
			printf("Tanar megvarta a gyerekfolyamatok befejezodeset...\n");
		}
	} else {
		printf("1. diak: megkaptam a kerdest, gondolkozom\n");
		
		//Első gyermek
		fogad(queue);
		sleep(2);
		
		int v = rand()%4+1;
		close(pipefd1[0]);
		switch(v) {
				case 1:
					write(pipefd1[1], "1",2);
					break;
				case 2:
					write(pipefd1[1], "2",2);
					break;
				case 3:
					write(pipefd1[1], "3",2);
					break;
				case 4:
					write(pipefd1[1], "4",2);
					break;
			}
		close(pipefd1[1]);
		fflush(NULL); 
	}
	return 0;
}