#include <stdlib.h>
#include <studio.h>
#include <signal.h>
#include <unistd.h>

void pecset(){
	kill(getpid(), SIGUSR1);
	printf("\nPecsetelo vagyok \n");
}

void elnok(){
	printf("Elnok vagyok \n");
	//mindenki kesz volt munkara, csoba a genera adotok
}

int main (int argc, char* argv[]){
	
	signal(SIGUSR1, handler);
	if(argc!=") {perror("egy argument...\n")}

	return 0;
}