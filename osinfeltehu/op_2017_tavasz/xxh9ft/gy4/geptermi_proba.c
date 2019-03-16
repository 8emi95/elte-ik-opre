#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

#include <signal.h>
	//
	// unnamed pipe example
	//
int x=0;
void handler(int signumber) {
printf("Signal with number %i has arrived\n", signumber);
    ++x;
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 



    // pipe
    int pipefd[2]; // unnamed pipe file descriptor array
    // 0 - olvas, 1 - ír
    pid_t adatellenorzo;
    char sz[100];  // char array for reading from pipe
    // pipe fv hívásnak a fork előtt kell lenni

    if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }



    // fork
    adatellenorzo = fork();	// creating parent-child processes
    if (adatellenorzo == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (adatellenorzo == 0)
    { // child 1 process
        kill(getppid(),SIGTERM);
        printf("adatellenorzo: Kesz a munkara\n");


        /**/
        close(pipefd[1]);  //Usually we close the unused write end

        printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0], sz, sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s", sz);
        printf("\n");

        close(pipefd[0]); // finally we close the used read end
        /**/
    }
    else
    {    // pre szulo process 
        pid_t pid2 = 0;
        pid2 = fork();

        if(pid2 != 0) {
            // szulo
            while(x<2) {}
            printf("Szulo: megkatpam ket jelzest\n");

            int voterNum = 0;
            voterNum = atoi(argv[1]);
            printf("Argument is %i\n", voterNum);

            int voters[voterNum];
            int i = 0;
            for(i=0; i<voterNum; ++i) {
                voters[i] = i+1;
            }

            /** /
            close(pipefd[0]); // Usually we close unused read end
            write(pipefd[1], "Hajra Fradi!", 13); // Fontos, hogy 13, méretnél egyel több, különben nem írja bele a végén a nullát, és nem lesz vége.
            close(pipefd[1]); // Closing write descriptor 
            printf("Szulo beirta az adatokat a csobe!\n");
            fflush(NULL); 	// flushes all write buffers (not necessary) // akkor kell, ha azt akarom, hogy mindenképp mindent kiírjon, ami az ideiglenes tárolóban van.
            wait();		// waiting for child process (not necessary)
            // try it without wait()
            /**/

            wait();
            wait();

            printf("Szulo: Befejeztem");
        } else {
            // gyerek 2
            kill(getppid(), SIGTERM);
            printf("gyerek1: Kesz a munkara\n");

            // itt is le kell zárni a cső író végét, hogy az olvasásánál megálljon.
        }
    }

    exit(EXIT_SUCCESS);	// force exit, not necessary
}


