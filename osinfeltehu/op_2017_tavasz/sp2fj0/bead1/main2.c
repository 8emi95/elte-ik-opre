#include <signal.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "quest.h"

int main(int argc, char *argv[])
{
	srand (time(NULL));
	//get the filename and check if it is openable
	char fileName[1000];
	printf("File name: "); scanf("%s",fileName);
	while( access( fileName, F_OK ) == -1 )
	{
		printf("That selection is invaild. Please try again. \n"); scanf("%s", fileName);
	}

	//creating pipes
	int MyPipeField[2];
	char buffer[18][1000];
	int MyPipeField2[2];
	int buffer2[12];
	int myPipe = pipe(MyPipeField);
	int myPipe2 = pipe(MyPipeField2);
	if(myPipe < 0)
	{
		perror("pipe(MyPipeFiled) error!");
		exit(-1);
	}

	if(myPipe2 < 0)
	{
		perror("pipe(MyPipeFailed2) error!");
		exit(-1);
	}
	
	//creating child
	pid_t child = fork();
	if(child < 0)
	{
		perror("fork() error");
		exit(-1);
	}

	//parent process
	if(child != 0)
	{	
		printf("%d. reporter was sent to %s\n", child, argv[1]);
		printf("MLSZ is waiting for the reporter to arrive at %s\n", argv[1]);
		kill(child,SIGCONT);
		finalizeForMlsz(fileName);
		char a[18][1000];
		listForMlsz(a);
		write(MyPipeField[1], a, sizeof(a));
		close(MyPipeField[1]);
		printf("Questions has been sent to %s\n", argv[1]);
		wait(NULL);
		read(MyPipeField2[0],buffer2,sizeof(buffer2));
		close(MyPipeField2[0]);
		printf("The answers has been arrived at MLSZ\n");
		printf("The results are: \n");
		int z=0;
		int u=0;
		while(z<18)
		{
			printf("%s", a[z]); z++;
			printf("%i marks ||| %s",buffer2[u],a[z]); u++; z++;
			printf("%i marks ||| %s",buffer2[u],a[z]); u++; z++;
			if(strcmp(a[z],"NULL\n") != 0) printf("%i marks ||| %s",buffer2[u],a[z]); z++; u++;
			if(strcmp(a[z],"NULL\n") != 0) printf("%i marks ||| %s",buffer2[u],a[z]); z++; u++;
			z++;
		}
	//child process
	}else{
		wait(NULL);
		printf("The reporter has logged in\n");
		close(MyPipeField[1]);
		read(MyPipeField[0],buffer,sizeof(buffer));
		printf("Questions has been arrived at %s\n", argv[1]);
		//in case you want to see the questions
		//int z = 0;
		//for(z;z<18;z++)
		//	printf("%s",buffer[z]);

		int fill[12] = {0};
		int numPeople = rand() % (20 + 1 - 10) + 10;
		int randf;
		int i = 0;
		while(i<numPeople)
		{	//first ans
			randf = rand() % 4;
			while( strcmp(buffer[1 + randf], "NULL\n") == 0) 
					randf = rand() % 4;
			fill[randf]++;
			//second ans
		        randf = rand() % 4;
			while( strcmp(buffer[7 + randf], "NULL\n")==0)
					randf = rand() % 4;
			fill[randf + 4]++;
			//third ans
			randf = rand() % 4;
			while(strcmp(buffer[13+ randf],"NULL\n")==0)
					randf = rand() % 4;
			fill[randf + 8]++;
			i++;
		}
		//i = 0;
		//for(i;i<12;i++) printf("%i ",fill[i]);
		printf("The quest has been filled by %i people\n", numPeople);
		close(MyPipeField[0]);
		write(MyPipeField2[1],fill,sizeof(fill));
		close(MyPipeField2[1]);
		printf("The answers has been sent back to MLSZ\n");
		kill(getpid(),SIGTERM);
	} 
	return 0;
}












