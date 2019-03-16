#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

void writestd(const char* str)
{
    write(STDOUT_FILENO, str, strlen(str));
}

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

void sig_handler(int signumber)
{
	/* SIGUSR2 = np */
	if (signumber == SIGUSR2)
		exit(0);
}

int fogadvegrol(int uzenetsor)
{
	struct uzenet uz;
	int status;
	
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
	if ( status < 0 ) perror("msgsnd");
	
	printf("Parent: a gyerek %s miatt halt!\n", uz.mtext);
	
	return 0;
}

char* ok = NULL;

int kuldvegrol(int uzenetsor)
{
	if (ok == NULL)
	{
		kill(getppid(), SIGUSR2);
		return 0;
	}
	
	kill(getppid(), SIGUSR1);
	int status;
	struct uzenet uz = { 5, "" };
	strcpy(uz.mtext, ok);
	status = msgsnd(uzenetsor, &uz, sizeof(uz.mtext), 0 );
	if ( status < 0 ) perror("msgsnd"); 
	return 0;
}

int kuld( int uzenetsor, int maxszerelveny ) 
{ 
	srand(time(NULL));
	int szerelveny = rand() % (maxszerelveny);
	
	struct uzenet uz = { 5, "" };
	sprintf(uz.mtext, "%d %d", szerelveny, maxszerelveny);
	
	int status;
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 );
	
	if ( status < 0 ) perror("msgsnd"); 
	return 0; 
} 
     
int fogad( int uzenetsor ) 
{ 
	struct uzenet uz; 
	int status; 
	
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
	if ( status < 0 ) perror("msgsnd"); 
	
	int szerelveny = 0;
	int maxszerelveny = 0;
	
	sscanf(uz.mtext, "%d %d", &szerelveny, &maxszerelveny);
	
	srand(time(NULL));
	int survived[maxszerelveny];
	int i = 0;
	while (i < maxszerelveny)
	{
		survived[i] = (rand() % 10) != 0;
		++i;
	}
	
	if (!survived[szerelveny])
	{
		ok = rand() % 10 == 0 ? "bananon csusztam el" : "zord telben lefagytam";
	}
	
	printf("Child: a %d. szerelveny vagyok, meg %d lesz utanam, valamint %s\n", szerelveny + 1, maxszerelveny - (szerelveny + 1), ok == NULL ? "teljesitettem az uzemet" : "nem teljesitettem az uzemet");

	return 0; 
} 

int main(int argc, char* argv[])
{
	pid_t child = fork();
	
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);
	
	int kulcs = ftok("data", 1); 
	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	
	if ( uzenetsor < 0 )
	{
		perror("msgget"); 
		return 1; 
	} 
	
	if (child > 0) /* parent */
	{
		kuld( uzenetsor, atoi(argv[1]) );  // Parent sends a message. 
		pause();
		fogadvegrol(uzenetsor);
		wait( NULL );
		
		int status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		if ( status < 0 ) perror("msgctl"); 
		return 0; 
	}
	else /* child */
	{
		int status = fogad( uzenetsor );
		if ( status < 0 ) perror("msgctl"); 
		kuldvegrol(uzenetsor);
		return 0;
	}
}