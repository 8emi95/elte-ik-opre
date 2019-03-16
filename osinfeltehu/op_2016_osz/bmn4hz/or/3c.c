#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	int id = getpid();
	int db = 3;
	for(int i = 0; i < db - 1; i++){
		if(getpid() == id){
			fork();
		}
	}
	printf("Hello %i vagyok %i gyereke!\n", getpid(),getppid());
	fflush(stdout);
	if(getpid() == id){
		int status;
		for(int i = 0; i < db - 1; i++){
			wait(&status);
		}
		printf("vegee :D");
	}
	return 0;
}