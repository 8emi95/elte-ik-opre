#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
//#include <time.h> //system time

#include <unistd.h> // for pipe()
#include <sys/time.h> //srand

#include <signal.h>

#define BUFFER_SIZE 256
char readBuffer[BUFFER_SIZE];

void handler(){
	printf("Telefonálás mkezdeményezése.\n");
}

void handler2(){
	printf("Utas: Jelzek a taxisnak.\n");
}

int generateIntBetween(int lowerBound, int upperBound){
    //srand(time(NULL)); //uncomment for real results
    int randInt = rand()%(upperBound - lowerBound) + lowerBound;
    printf("    generated: %d\n", randInt);
    return randInt;
}

void createPipe(int* descriptor){
    if (pipe(descriptor) == -1){
        perror("Pipe error.\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc,char *argv[]){
	//taxis - központ
    int parentToChild1[2];
    createPipe(parentToChild1);
    int child1ToParent[2];
    createPipe(child1ToParent);

    //utas - központ
    int parentToChild2[2];
    createPipe(parentToChild2);
    int child2ToParent[2];
    createPipe(child2ToParent);

    //taxis - utas
    int child1ToChild2[2];
    createPipe(child1ToChild2);
    int child2ToChild1[2];
    createPipe(child2ToChild1);

    int readEnd = 0;
    int writeEnd = 1;

	printf("\n__________________________________________________\n");
    pid_t child1 = fork();
    if (child1 < 0){perror("Fork error\n"); exit(1);}
    if(child1 > 0){ //parent - központ
        pid_t child2 = fork();
        if (child2 < 0){perror("Fork error\n"); exit(1);}
        if(child2 > 0){ //parent - központ
            //pipe setup
            close(parentToChild1[readEnd]);
            close(parentToChild2[readEnd]);

            close(child1ToParent[writeEnd]);
            close(child2ToParent[writeEnd]);

            close(child1ToChild2[readEnd]);
            close(child1ToChild2[writeEnd]);
            close(child2ToChild1[readEnd]);
            close(child2ToChild1[writeEnd]);

			//utastól: utasra vár
			read(child1ToParent[readEnd], &readBuffer, sizeof(readBuffer));
			printf("Központ: utas üzenete: %s\n", readBuffer);
		
			//utastól: lakcím
			read(child1ToParent[readEnd], &readBuffer, sizeof(readBuffer));
			char lakcim[BUFFER_SIZE];
			strcpy(lakcim, readBuffer);
			printf("Központ: az utas lakcíme: %s\n", lakcim);

			//taxisnak: utas lakcíme
			printf("Központ: elküldöm a taxisnak az utas lakcímét.\n");
			write(parentToChild2[writeEnd], lakcim, sizeof(lakcim));
			
			//taxisnak: utas telefonszáma(pid)
			printf("Elküldöm a taxisnak az utas telefonszámát: %d.\n", child1);
			write(parentToChild2[writeEnd], &child1, sizeof(child1));
			
			//taxistól: elindult
			read(child2ToParent[readEnd], readBuffer, sizeof(readBuffer));
			char msg[BUFFER_SIZE];
			strcpy(msg, readBuffer);
			printf("Központ: Taxis üzenete: %s\n", msg);
			
			//megvárjuk amíg a mindenki más végez
            int status, waitId;
            while(waitId = wait(&status) > 0);  

            printf("Központ bezár.\n");
			
			close(parentToChild1[writeEnd]);
            close(parentToChild2[writeEnd]);

            close(child1ToParent[readEnd]);
            close(child2ToParent[readEnd]);
        }
        else{ //child2 - taxis
            //pipe setup
            close(parentToChild1[readEnd]);
            close(parentToChild1[writeEnd]);
            close(child1ToParent[readEnd]);
            close(child1ToParent[writeEnd]);

            close(parentToChild2[writeEnd]);
            close(child2ToParent[readEnd]);

            close(child1ToChild2[writeEnd]);
            close(child2ToChild1[readEnd]);

            //központtól: utas lakcíme
			char lakcim[BUFFER_SIZE];
			read(parentToChild2[readEnd], lakcim, sizeof(lakcim));
			printf("Taxis: utas lakcíme: %s\n", lakcim);
			
			//központtól: utas telefonszáma
			int telefonszam;
			read(parentToChild2[readEnd], &telefonszam, sizeof(telefonszam));
			printf("Taxis: utas telefonszáma: %d\n", telefonszam);
			
			//központnak: elindultam
			char msg[] = "Elindultam";
			write(child2ToParent[writeEnd], msg, sizeof(msg));
			
			//címre érkezett: jelez
			printf("Taxis: megérkeztem, telefonálok az utasnak.\n");
			int telefonszamom = getpid();
			write(child2ToChild1[writeEnd], &telefonszamom, sizeof(telefonszamom));
			
			read(child1ToChild2[readEnd], &readBuffer, sizeof(readBuffer));
			kill(telefonszam, SIGUSR1);
			
			//cím fogadása
			printf("Taxis: várok az úticélra.\n");
			//telefon nem rezgőn
			signal(SIGUSR2, handler2);
			char msg2[] = "ok";
			write(child2ToChild1[writeEnd], msg2, sizeof(msg2));
			pause();
			int cim;
			read(child1ToChild2[readEnd], &cim, sizeof(cim));
			printf("Taxis: az utas címe: %d\n", cim);
			int ar = cim*300 + 700;
			printf("Taxis: az utazás díja: %d\n", ar);

            //finishing up
            printf("Taxis végzett.\n");
			
            close(parentToChild2[readEnd]);
            close(child2ToParent[writeEnd]);

            close(child1ToChild2[readEnd]);
            close(child2ToChild1[writeEnd]);
        }
    }
    else{ //child1 - utas
        //pipe setup
        close(parentToChild2[readEnd]);
        close(parentToChild2[writeEnd]);
        close(child2ToParent[readEnd]);
        close(child2ToParent[writeEnd]);

        close(child1ToParent[readEnd]);
        close(parentToChild1[writeEnd]);

        close(child1ToChild2[readEnd]);
        close(child2ToChild1[writeEnd]);

		//bejelentkezik
		char msg[] = "utaznék";
		printf("Utas: bejelentkezek.\n");
		write(child1ToParent[writeEnd], msg, sizeof(msg));
		
		//lakcím küldése
		printf("Utas: elküldöm a lakcímem.\n");
		write(child1ToParent[writeEnd], argv[1], sizeof(argv[1]));

		//vár a taxisra
		printf("Utas: várok a taxisra.\n");
		
		//telefon nem rezgőn
		signal(SIGUSR1, handler);
		write(child1ToChild2[writeEnd], msg, sizeof(msg));
		pause();
		
		//taxis telefonszáma
		int taxis;
		read(child2ToChild1[readEnd], &taxis, sizeof(taxis));
		
		//cím bemondása
		read(child2ToChild1[readEnd], &readBuffer, sizeof(readBuffer));
		int cim = generateIntBetween(1, 20);
		printf("Utas: a címem: %d\n", cim);
		write(child1ToChild2[writeEnd], &cim, sizeof(cim));
		kill(taxis, SIGUSR2);

        //finishing up
        printf("Utas végzett.\n");

        close(child1ToParent[writeEnd]);
        close(parentToChild1[readEnd]);

        close(child1ToChild2[writeEnd]);
        close(child2ToChild1[readEnd]);
    }
	
	return 0;
}