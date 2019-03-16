#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

void handler(int signumber);

int main(int argc, char *argv[]){
	signal(SIGTERM,handler);
	srand(time(NULL));

    int n = atoi(argv[1]);

    /* Csovezetek letrehozasa */
	int MyPipeFiel[2];

	int MyPipe = pipe(MyPipeFiel);
	if (MyPipe < 0)
	{
		perror("\nCsovezetek megnyitasa sikertelen!\n");
		exit(-1);
	}

    pid_t child_a, child_b;

    child_a = fork();

    if (child_a == 0) {
        /* Child A code */
        // [PECSETELO]
        sleep(0.1);
        printf("[PECSETELO]: Bejelentkeztem\n");
        kill(getppid(), SIGTERM);
    } else {
        child_b = fork();
        if (child_b == 0) {
            /* Child B code */
            // [ ELLENOR ]
            sleep(0.2);
            printf("[ ELLENOR ]: Bejelentkeztem\n");
            kill(getppid(), SIGTERM);
            pause();
            
            int buffer[n];
            read(MyPipeFiel[0], buffer, sizeof(buffer));
            printf("[ ELLENOR ]: Megkaptam az azonositokat: \n");
            int i;
            for(i = 0; i < n; i++){
                printf("%d, ", buffer[i]);
            }
            printf("\n");
            kill(getppid(), SIGTERM);
        } else {
            /* Parent Code */
            // [  ELNOK  ]
            printf("[  ELNOK  ]: Varok a tagok jelentkezesere\n");
            pause();
            pause();
            
            printf("[  ELNOK  ]: Itt van mindenki, mehetünk\n");

            int azon[n];
            int i;
            for(i = 0; i < n; i++){
                azon[i] = rand() % 1000;
            }
            write(MyPipeFiel[1], azon, sizeof(azon));
            printf("[  ELNOK  ]: Atadtam az azonositokat, varok\n");
            kill(child_b, SIGTERM);
            
            pause();
            printf("[  ELNOK  ]: Gyerek vegzett, 1. feladat kesz\n");
        }
    }
    return 0;
}

void handler(int signumber){
    //printf("Megkaptam a %d szamu signalt", signumber);
}