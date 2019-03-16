/*
    SIGABRT - "abort", abnormal termination.
    SIGFPE - floating point exception.
    SIGILL - "illegal", invalid instruction.
    SIGINT - "interrupt", interactive attention request sent to the program.
    SIGSEGV - "segmentation violation, invalid memory access.
    SIGTERM - "terminate", termination request sent to the program.
*/

#include <unistd.h>	//Fork
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit
//Új
#include <signal.h>   //Signalhoz kell

void handler(int signumber)
{
  if(signumber == SIGTERM) printf("[Szülő] - SIGTERM érkezett\n");
  if(signumber == SIGINT) printf("[Szülő] - SIGINT érkezett\n");
  if(signumber == SIGSEGV) printf("[Szülő] - SIGSEGV érkezett.\n");
};

void sendSignal(int i)
{
	pid_t childPid = getpid();
	switch(i)
	{
		case 0 :
			printf( "[Gyerek] pid: %d - SIGTERM SZIGNÁL elküldve.\n", childPid);
			kill(childPid, SIGTERM);
		break;
		case 1 :
			printf( "[Gyerek] pid: %d - SIGINT SZIGNÁL elküldve.\n", childPid);
			kill(childPid, SIGINT);
		break;
		case 2 :
			printf( "[Gyerek] pid: %d - SIGSEGV SZIGNÁL elküldve.\n", childPid);
			kill(childPid, SIGSEGV);
		break;
		default :
			printf( "[Gyerek] pid: %d - Nincs szignál.\n", childPid);
	}
}

int main(int argc, char* argv[])
{
	int n = 3;
	pid_t childs[n];
	pid_t parent = getpid();
    printf( "[Szülő] pid: %d\n",  parent);
	
	signal(SIGTERM,handler);
	signal(SIGINT,handler);
	
	int i;
    for ( i = 0; i < n; i++ )
	{
		pid_t pid = fork();
		if(getpid() == parent) childs[i] = pid;
        if ( pid == 0 )
        {
			pid_t childPid = getpid();
			pid_t myParentPid = getppid();
			printf( "[Gyerek] pid: %d - szülő: %d\n", childPid, getppid() );
			sendSignal(i);
            exit( 0 );
        }
	}
    for ( i = 0; i < n; i++ )
	{
		waitpid( childs[i] );
		printf( "[Szülő] pid: %d - %d gyerek leállt.\n", parent, childs[i]);
	}
	printf( "[Szülő] pid: %d - leáll.\n", parent);
}

