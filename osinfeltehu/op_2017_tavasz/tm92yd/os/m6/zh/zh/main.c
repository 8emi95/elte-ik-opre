#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>

struct uzenet { 
	long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	char mtext [ 1024 ]; 
};

// uzenetkuldes
int kuld( int uzenetsor ) {
	const char *kerdes = "\n5-os, vagyok 6-os jegyet szeretnetek?\n";
	struct uzenet uz = { 5, "Ez egy uzenet" };
	strcpy(uz.mtext, kerdes);

	int status; 
	// uzenetsor
	status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	if ( status < 0 ) { 
		perror("msgsnd");
	}
	return 0; 
}

// uzenet fogadas
int fogad( int uzenetsor ) {
	struct uzenet uz; 
	int status; 
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
	if (status < 0) {
		perror("msgsnd");
	}
	else
		printf( "A kapott kerdes:  %s", uz.mtext ); 
	return 0; 
}

int main(int argc, char* argv[]) 
{
	srand(time(NULL));
	int uzenetsor, status;
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
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if (uzenetsor < 0) { 
		perror("msgget"); 
		return 1; 
	}
	

	pid_t child = fork();
	if (child > 0) {
		pid_t child2 = fork();
		if(child2 == 0) { // Child 2
			printf("2. hallgato vagyok, megkaptam a kerdest, kicsit gondolkozom\n");
			fogad(uzenetsor);
			
			sleep(1);
			int valasz = rand()%4+1;
		valasz = rand()%4+1;
			close(pipefd2[0]);
			if(valasz==1)
				write(pipefd2[1], "1",2);
			else if(valasz == 2)
				write(pipefd2[1], "2",2);
			else if (valasz == 3)
				write(pipefd2[1], "3",2);
			else if (valasz == 4)
				write(pipefd2[1], "4",2);
			close(pipefd2[1]);
			fflush(NULL); 
		}

		else { // Parent
			printf("A tanar vagyok, kuldom a kerdeseket.\n");
				
			// Uzenetsor kuldese
			kuld(uzenetsor);
			kuld(uzenetsor);
			
			//status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			//if ( status < 0 ) perror("msgctl"); 

			int status;
			wait(&status);
			close(pipefd1[1]);  //Usually we close the unused write end
			close(pipefd2[1]);  //Usually we close the unused write end
			char valasz1 [2] = "";
			char valasz2 [2] = "";
			//int valasz2;
               read(pipefd1[0],valasz1,sizeof(valasz1)); 
			   read(pipefd2[0],valasz2,sizeof(valasz2)); 
			 printf("Az elso valasz %s\n", valasz1);
			 printf("A masodik valasz %s\n",valasz2);
			printf("A tanar megvarta a gyerekfolyamatok veget\n");
		}
	}

	else { // Child 1
		int i;
		printf("1. hallgato vagyok, megkaptam a kerdest,kicsit gondolkozom\n");
		fogad(uzenetsor);
		
		sleep(1);
		int valasz = rand()%4+1;
		valasz = rand()%4+1;
		valasz = rand()%4+1;
		close(pipefd1[0]);
		if(valasz==1)
			write(pipefd1[1], "1",2);
		else if(valasz == 2)
			write(pipefd1[1], "2",2);
		else if (valasz == 3)
			write(pipefd1[1], "3",2);
		else if (valasz == 4)
			write(pipefd1[1], "4",2);
		close(pipefd1[1]);
		fflush(NULL); 
	}
	return 0;
}