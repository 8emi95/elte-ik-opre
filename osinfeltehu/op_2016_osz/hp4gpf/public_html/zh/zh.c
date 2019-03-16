#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h> // for pipe()
 
int length(char * str)
{
 int l=0;
 //elõször a mutatót lépteti, aztán összehasonlít, majd a mutató értékét móddosítja
 while (*str++!=0)l++; //*str points to the next character
 return l;
}
 
int main(int argc, char *argv[]){
	printf("Neved : %s\n",argv[1]);
	
	int pipeTR[2]; // unnamed pipe file descriptor array (egyik a csõ eleje, a másik a vége) 1 író vég, 0 az olvasó vég
    int pipeVI[2];
	pid_t vizsgazo;
	char sz[100];  // char array for reading from pipe
	int randValasz = rand()%5;
	int namesize = length(argv[1]);
    //Ha valami hiba van akkor -1 
    if (pipe(pipeTR) == -1) {
		perror("Hiba a pipe nyitaskor! TRENER");
		exit(EXIT_FAILURE);
    }
    if (pipe(pipeVI) == -1) {
		perror("Hiba a pipe nyitaskor! VIZSGAZO");
		exit(EXIT_FAILURE);
    }
    vizsgazo = fork();	// creating parent-child processes
    if (vizsgazo == -1) {
		perror("Fork hiba: NO VIZSGAZO");
		exit(EXIT_FAILURE);
	}
	if (vizsgazo == 0) {
		sleep(3);	// sleeping a few seconds, not necessary
        close(pipeTR[1]);  //Usually we close the unused write end
		printf("Vizsgazo olvassa a feladatot!\n");
        read(pipeTR[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Olvasott: %s",sz);
	    printf("\n");
		if(rand()%5 == 0){
			write(pipeVI[1],"Nem sikerult megoldani", 22 );
		}else{
			write(pipeVI[1],"Sikerult megoldani", 18);
		}
        close(pipeTR[0]); // finally we close the used read end
   } 
   else 
   {    // szulo process 
		printf("Szulo indul!\n");
		close(pipeTR[0]); //Usually we close unused read end
		write(pipeTR[1], "Csinalj csovet!\t",16);
		write(pipeTR[1], argv[1],namesize);
		write(pipeTR[1], "\t3",2);
		close(pipeTR[1]); // Closing write descriptor 
		printf("Szulo beirta az adatokat a csobe!\n");
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait();		// waiting for child process (not necessary)
		read(pipeVI[0], sz, sizeof(sz));
		printf("Vizsgazo valasza: %s", sz);
		printf("\n");
		// try it without wait()
	    printf("Szulo befejezte!");	
    }
	
    /*if (vizsgazo == 0) {		    	// child process
	    //sleep(3);	// sleeping a few seconds, not necessary
        //close(pipeTR[1]);  //Usually we close the unused write end
        printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipeTR[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Gyerek feladata: %s",sz);
		read(pipeTR[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Gyerek neve: %s",sz);
		read(pipeTR[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Gyerek sorszama: %s",sz);
	    printf("\n");
		randValasz = rand() % 5;
		if(randValasz == 0){
			printf("Bena vagyok nem ment a feladat.\n");
			write(pipeVI[1], "Bena vagyok nem ment a feladat.",31);
		}else{
			printf("Megcsinaltam, johet a kovetkezo.\n");
			write(pipeVI[1], "Megcsinaltam, johet a kovetkezo.",32);
		}
        //write(pipeVI[1], "Hajra Vasas!",14);
        close(pipeVI[1]);
        close(pipeTR[0]); // finally we close the used read end
    }else{    // szulo process 
        //sleep(3); //ha itt alszik, akkor a gyerek próbál belõle olvasni.
        printf("TRENER feladatot ad!\n");
        //close(pipeTR[0]); //Usually we close unused read end
        write(pipeVI[1], "Csinalj csovet!",15);
		write(pipeVI[1], argv[1],namesize);
		write(pipeVI[1], "3",1);
		
        read(pipeTR[0], sz, sizeof(sz));
        printf("Ez a valasz %s\n", sz);
        close(pipeVI[1]); // Closing write descriptor 
        printf("Szulo beirta az adatokat a csobe!\n");
        fflush(NULL); 	// flushes all write buffers (not necessary)
        wait();		// waiting for child process (not necessary)
		// try it without wait()
	    printf("Szulo befejezte!");	
	}*/
	exit(EXIT_SUCCESS);	// force exit, not necessary
}