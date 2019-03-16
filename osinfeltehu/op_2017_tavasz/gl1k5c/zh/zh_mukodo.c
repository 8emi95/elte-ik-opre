#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>		//fork
#include <sys/wait.h>	//waitpid
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 

int main(int argc, char** argv)
{
	printf("\n*********************** Fuvarszervezo ***********************\n\n");

	int childParentPipe[2];	
	
	//Utas létrehozása
	pid_t passangerPid = fork();
	if (passangerPid<0){perror("A fork hivas sikertelen volt!\n"); exit(1);}
	
	if (passangerPid > 0)			//Szülõ
	{
		//close(childParentPipe[1]);
		char* receivedAddress;
		read(childParentPipe[0],receivedAddress,strlen(argv[1])+1);
		printf("Az utas cime: %s\n\n",receivedAddress);
		
		waitpid(passangerPid,NULL,0);
	}
	else						//Utas
	{
		close(childParentPipe[0]);
		char* address = argv[1];
		write(childParentPipe[1],address,strlen(argv[1])+1);
	}

	return 0;
}