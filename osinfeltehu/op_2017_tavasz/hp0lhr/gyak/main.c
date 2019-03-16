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

int sigCounter = 0;
char readBuffer[128];

void counter(){
    printf("%d. jelzés megérkezett az elnökhöz.\n",++sigCounter);
}

void createPipe(int* descriptor){
    if (pipe(descriptor) == -1){
        perror("Pipe error.\n");
        exit(EXIT_FAILURE);
    }
}

int generateIntBetween(int lowerBound, int upperBound){
    //srand(time(NULL)); //uncomment for real results
    int randInt = rand()%(upperBound - lowerBound) + lowerBound;
    printf("    generated: %d\n", randInt);
    return randInt;
}

int main(int argc,char *argv[]){
    //signal(SIGUSR1, counter);

    //adatellenőrző - elnök
    int parentToChild1[2];
    createPipe(parentToChild1);
    int child1ToParent[2];
    createPipe(child1ToParent);

    //lapkezelő - elnök
    int parentToChild2[2];
    createPipe(parentToChild2);
    int child2ToParent[2];
    createPipe(child2ToParent);

    //adatellenőrző - lapkezelő
    int child1ToChild2[2];
    createPipe(child1ToChild2);
    int child2ToChild1[2];
    createPipe(child2ToChild1);

    int readEnd = 0;
    int writeEnd = 1;

    pid_t child1 = fork();
    if (child1 < 0){perror("Fork error\n"); exit(1);}
    if(child1 > 0){ //parent - elnök
        pid_t child2 = fork();
        if (child2 < 0){perror("Fork error\n"); exit(1);}
        if(child2 > 0){ //parent - elnök
            //pipe setup
            close(parentToChild1[readEnd]);
            close(parentToChild2[readEnd]);

            close(child1ToParent[writeEnd]);
            close(child2ToParent[writeEnd]);

            close(child1ToChild2[readEnd]);
            close(child1ToChild2[writeEnd]);
            close(child2ToChild1[readEnd]);
            close(child2ToChild1[writeEnd]);

            //waiting for checkins
            signal(SIGUSR1, counter);
            char msg[] = "ok";
            write(parentToChild1[writeEnd], msg, sizeof(msg));
            write(parentToChild2[writeEnd], msg, sizeof(msg));
            while (sigCounter < 2){
                pause();
            }

            //sending ids to adatellenőrző
            int voterCount = atoi(argv[1]);
            write(parentToChild1[writeEnd], &voterCount, sizeof(voterCount));

            int i;
            for (i = 0; i < voterCount; ++i){
                int id = generateIntBetween(0, 1024);
                write(parentToChild1[writeEnd], &id, sizeof(id));
            }

            //waitpid(-1, NULL, 0); //wait for all children to finish
            /*int waitCount = 0;
            while (waitCount < 2){
                wait();
                ++waitCount;
            }*/
            /*int status;
            wait(&status);*/

            int status, waitId;
            while(waitId = wait(&status) > 0);  

            printf("Elnök hazamegy.\n");
        }
        else{ //child2 - lapkezelő
            //pipe setup
            close(parentToChild1[readEnd]);
            close(parentToChild1[writeEnd]);
            close(child1ToParent[readEnd]);
            close(child1ToParent[writeEnd]);

            close(parentToChild2[writeEnd]);
            close(child2ToParent[readEnd]);

            close(child1ToChild2[writeEnd]);
            close(child2ToChild1[readEnd]);

            //signal ready
            //wait for parent to install handler
            read(parentToChild2[readEnd], &readBuffer, sizeof(readBuffer));
            kill(getppid(), SIGUSR1);

            //finishing up
            printf("Lapkezelő végzett.\n");

            close(parentToChild2[readEnd]);
            close(child2ToParent[writeEnd]);

            close(child1ToChild2[readEnd]);
            close(child2ToChild1[writeEnd]);
        }
    }
    else{ //child1 - adatellenőrző
        //pipe setup
        close(parentToChild2[readEnd]);
        close(parentToChild2[writeEnd]);
        close(child2ToParent[readEnd]);
        close(child2ToParent[writeEnd]);

        close(child1ToParent[readEnd]);
        close(parentToChild1[writeEnd]);

        close(child1ToChild2[readEnd]);
        close(child2ToChild1[writeEnd]);

        //signal ready
        //wait for parent to install handler
        read(parentToChild1[readEnd], &readBuffer, sizeof(readBuffer));
        kill(getppid(), SIGUSR1);

        //receiving ids
        int voterCount;
        read(parentToChild1[readEnd], &voterCount, sizeof(voterCount));

        int i;
        for (i = 0; i < voterCount; ++i){
            int id;
            read(parentToChild1[readEnd], &id, sizeof(id));
            printf("AE: %d. id: %d\n", i + 1, id);
        }

        //finishing up
        printf("Adatellenőrző hazamegy.\n");

        close(child1ToParent[writeEnd]);
        close(parentToChild1[readEnd]);

        close(child1ToChild2[writeEnd]);
        close(child2ToChild1[readEnd]);
    }

   return 0;
}