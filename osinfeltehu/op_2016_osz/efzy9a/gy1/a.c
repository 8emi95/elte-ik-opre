#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// fork
#include <sys/wait.h>// waitpid
#include <errno.h>


// gyerekfolyamatok megvarasa
int main(){
	//pid_t en = fork();
	pid_t child = fork();// forks make a copy of variables
	if(child > 0){
	//if(en == getpid()){
		printf("szulo\n");
	}
	else{
		printf("gyerek\n");
	}
	
	// printf("Hello %i\n", getpid());
	
	return 0;
}