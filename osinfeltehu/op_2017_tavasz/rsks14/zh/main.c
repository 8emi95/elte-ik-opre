#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

int main(int argc, char *argv[]){
	//int gyerek[2];
		
	if (argc == 1) {
		printf("Nem adott meg cimet --> ures az argumentum \n");
		return -1;
	}
	
	int taxiT[2];
	pid_t cpid;
	char fogad[sizeof argv[1]];
	
	if (pipe(taxiT) == -1) {
        perror("hiba");
		exit(EXIT_FAILURE);
    }
	
	cpid = fork();
	if (cpid == -1){
        perror("fork hiba");
        exit(EXIT_FAILURE);
    }
	
	if(cpid == 0){
		close(taxiT[0]);
		printf("Az utas ezt a cimet adta meg: %s \n",argv[1]);
		write(taxiT[1],argv[1], sizeof argv[1]);
		close(taxiT[1]);
		printf("Az utas megadta a cimet! \n");
		fflush(NULL);
	}else{
		close(taxiT[1]);
		read(taxiT[0],fogad,sizeof(fogad));
		printf("Ezt a cimet kapta meg a kozpont: %s \n",fogad);
		close(taxiT[0]);
	}
	exit(1);
}