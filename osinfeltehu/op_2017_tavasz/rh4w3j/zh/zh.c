#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


const char* strPipe1Name = "StadionPipe1";
//const char* strPipe2Name = "StadionPipe2";
//const char* strPipe3Name = "StadionPipe3";

void InitPipes(){
	int result;
	result = mkfifo(strPipe1Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}
	/*
	result = mkfifo(strPipe2Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}
	result = mkfifo(strPipe3Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}*/
};

void DestroyPipes(){
	unlink(strPipe1Name);
//	unlink(strPipe2Name);
//	unlink(strPipe3Name);
}

void Children1Main(char cim[])
{
		
		
		
		
}



int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{/*
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,(argv[i]));
 }*/
	InitPipes();
	
	char argumentem[100];
	pid_t p;
	p = fork();	
		if (p==0) //gyermek
	{
		
		//Children1Main(argumentem);
		printf(">>>child1 start<<<\n");
		int cso = open(strPipe1Name,O_WRONLY);
		write(cso, argv[1], 100);
		close(cso);
		printf("Mehetunk anyu\n");
		sleep(3);
		//
		
	}
	else 
	{
		printf(">>>Szulo start<<<\n");
		//sleep(2);	
	int start = open(strPipe1Name,O_RDONLY);
	char rdy1[100];
	int rdy=read(start,&rdy1, 100);
	if(rdy<0)
	{
		printf("Connect error\n");
	}
	if(rdy>=0)
	{
		printf("%s\n",rdy1);
	}
	close(start);
		DestroyPipes();
		wait(NULL);
		printf("BefejeztemSZ\n");
	}
	//kill(p,SIGKILL);
	printf("BefejeztemGY\n");
 
 return 0;
}
