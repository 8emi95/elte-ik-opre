#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <string.h>

struct uzenet { 
	long mtype;
	char mtext [ 1024 ]; 
}; 

int kuld(int uzenetsor) 
{ 
	const struct uzenet uz = { 5, "Ez a kerdes?" }; 
	int status; 

	status = msgsnd( uzenetsor, &uz, strlen( uz.mtext ) + 1 , 0 ); 
	if ( status < 0 ) 
	  perror("msgsnd 1"); 
	return 0; 
}

int fogad( int uzenetsor ) 
{ 
	struct uzenet uz; 
	int status; 
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
	if ( status < 0 ) 
		perror("msgsnd 2"); 
	else
		printf( "A napi kerdes: %s\n", uz.mtext ); 
	return 0; 
} 

int main(int argc, char* argv[])
{
	pid_t child; 
	int uzenetsor; 
	key_t kulcs; 

	srand(time(NULL));
	
	kulcs = ftok(argv[0],1); 
	printf ("A kulcs az uzenethez: %d\n",kulcs);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	} 
	
	int pipefd[2];
	char sz[100]; 
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(1);
	}
	
	int pipefd2[2];
	char sz2[100]; 
	if (pipe(pipefd2) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(1);
	}
	
	int status;
	pid_t hallgato1=fork();
	pid_t hallgato2=fork();
	if (hallgato1<0){perror("HIBA\n"); exit(1);}
	if (hallgato2<0){perror("HIBA\n"); exit(1);}
	
	if (hallgato1>0) //Az előadó
	{
		waitpid(hallgato1,&status,0);
		waitpid(hallgato2,&status,0);
		//printf("Én vagyok az előadó???! \n");
		
		//sleep(5);
		kuld( uzenetsor );
		wait( NULL );
		kuld( uzenetsor );
		wait( NULL );
	}
	
	if ((hallgato1>0)&&(hallgato2>0)) //Az előadó
	{
		waitpid(hallgato1,&status,0);
		waitpid(hallgato2,&status,0);
		printf("Én vagyok az előadó! \n");
		
		/*/sleep(5);
		kuld( uzenetsor );
		wait( NULL );
		kuld( uzenetsor );
		wait( NULL );*/
		
		sleep(3);
		close(pipefd[1]);
		read(pipefd[0],sz,sizeof(sz));
		printf("+HallgatoValasza: %s\n",sz);
		close(pipefd[0]);
		
		close(pipefd2[1]);
		read(pipefd2[0],sz2,sizeof(sz2));
		printf("+HallgatoValasza: %s\n",sz2);
		close(pipefd2[0]);
		
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		if ( status < 0 ) perror("msgctl"); 
	}
	if ((hallgato1==0)&&(hallgato2>0))//Hallgato 1
	{
		sleep( 1 ); 
		printf("Hallgato 1 \n");
		
		//+++ÜzenetFogad
		fogad( uzenetsor );
		
		//+++Random szám
		char snum[5];
		srand(time(NULL));
		int r=(rand()%(4-1))+1;
		sprintf(snum,"%d",r);
		strcat(snum,"-hallgato1");
		
		//+++Cső küld
		close(pipefd[0]);
		write(pipefd[1], snum,13);
		close(pipefd[1]);
		fflush(NULL);
		wait(NULL);
	}

	if ((hallgato1>0)&&(hallgato2==0)) //Hallgato 2
	{
		sleep( 1 ); 
		printf("Hallgato 2 \n");
		
		//+++ÜzenetFogad
		fogad( uzenetsor );
		
		//+++Random szám
		char snum[5];
		srand(time(NULL));
		int r=(rand()%(4-1))+1;
		sprintf(snum,"%d",r);
		strcat(snum,"-hallgato2");
		
		//+++Cső küld
		close(pipefd2[0]);
		write(pipefd2[1], snum,13);
		close(pipefd2[1]);
		fflush(NULL);
		wait(NULL);
	}
	return 0;
}