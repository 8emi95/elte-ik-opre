#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h> 

void main( int argc, const char* argv[] )
{
	// int uzenetsor;
	// key_t kulcs; 
	// kulcs = ftok(argv[0],1); 
	// uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 

	int pid;
	int pid2;
	int childwrite[2];
	
	pipe(childwrite);
	
	pid = fork();
	if (pid == 0) //utas
	{
		int i;
		char str [100];
		strcpy(str,argv[1]);
		for(i = 2; i < argc;i++)
		{
			strcat(str," ");
			strcat(str,argv[i]);
		}
		
		sleep(1);
		close(childwrite[0]);
		write(childwrite[1], str, 100);
		
		close(childwrite[1]);
		
	}
	else //kozpont
	{
		char address[100];
		close(childwrite[1]);
		read(childwrite[0], address, sizeof(address));
		
		close(childwrite[0]);
		
		pid2 = fork();
		if(pid2 == 0) //auto
		{
			// char passenger_address [100];
			// int phone_number;
			
			// msgrcv( uzenetsor, passenger_address, sizeof(passenger_address),0, 0 );
			// msgrcv( uzenetsor, &phone_number, sizeof(phone_number),0, 0 );
			
			// printf("Taxis: Az utas cime: %s \n",passenger_address);
			// printf("Taxis: Az utas telefonszáma: %d \n}",phone_number);
			
			// msgsnd( uzenetsor, "Elindultam az utasert", 22 , 0 );
		}
		else //kozpont
		{
			printf("Kozpont: Az utas cime: %s \n",address);
			
			// msgsnd( uzenetsor, &address, sizeof(address) , 0 );
			// msgsnd( uzenetsor, &pid, sizeof(pid) , 0 );
			
			// char message[100];
			// msgrcv( uzenetsor, message, sizeof(message),0, 0 );
			
			wait(NULL);
			wait(NULL);
		}
	}
}