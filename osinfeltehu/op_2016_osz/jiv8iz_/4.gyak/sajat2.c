#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //perror, errno
#include <unistd.h> // fork
#include <sys/wait.h> //waitpid

//Feladat: pontosan 3 processz legyen??? 
int main() {
	int value = 2;
	pid_t child = fork();
	
	if(child<0){
		//errorvalami nem sikerült a forknál
		printf("Valami hiba\n");
	} else if(child >0) {  // gyerek folyamatban vagyunk
		pid_t child2 =fork();
		pid_t mypid = getpid(); // saját pid lekérdezése
		pid_t parentpid = getppid(); // szülő pid lekérdezése
		printf("Pidem: %i\n", mypid);
		

		
	}else {  // chiild =0 akkor nem vagyok a gyerekben- parentben vagyok
		
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("Pidem: %i\n", mypid);
		
		
	}
	
	return 0;
}
