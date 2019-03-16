#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h> // Fork!
#include <errno.h>
#include <time.h>
#include <sys/types.h>

// Avoid implicit declaration errors for wait
#include <sys/wait.h>

void signalHandler(int signumber) {
	printf("Signal with number %i has arrived in %d \n", signumber, getpid());
	// printf("Signal handler ends \n");
}

char** explode(char* buffer, int *numParts) {
	char ** result = NULL;
	char * pch;
	int numberOfParts = 0;
	*numParts = 0;

	pch = strtok(buffer, " ");

	while (pch != NULL) {
		result = (char **) realloc(result, sizeof (char*) * ++numberOfParts); // This reallocates the space for result everytime the while runs. Pretty slow, but fair enogh for now...
		if (NULL == result) {
			printf("Nem sikerült elég memóriát allokálni. A progam bezár. \n");
			exit(-1);
		}

		result[numberOfParts - 1] = pch;

		pch = strtok(NULL, " ");
	}

	*numParts = numberOfParts;

	return result;
}

int main(int argc, char **argv)
{
    if(argc > 10) {
        perror("Túl sok argumentum (max 10 név)!");
        exit(EXIT_FAILURE);
    }

	int numParts; // For my explode function

    // Initalising variable for pipe:
    int pipefd[2]; // array of integers
    // init for singals:
    sigset_t signalSet;
    // Making the set empty (initing with 0)
    sigemptyset(&signalSet);
    // Adding USR signal 
    sigaddset(&signalSet, SIGUSR1);

    // Attaching handlers:
    signal(SIGUSR1, signalHandler); // Easy peasy.

    // initalising my things for handling messages:
    char theMessenger[BUFSIZ];
    memset(&theMessenger, '\0', sizeof(theMessenger)); // Init done.

    // mkay so far so good, let's init that pipe:
    if(pipe(pipefd) == -1) {
        perror("Pipe nyitás hiba!");
        exit(EXIT_FAILURE);
    }

    // Multiple child:
    printf("argc: %d \n", argc);

    // I am going to store in this array that who has failed:
    char failed[BUFSIZ]; // Lets make it just "big enough"
    char failedSecondRound[BUFSIZ];
    char success[BUFSIZ];
    char successSecondRound[BUFSIZ];

    memset(&failed, '\0', sizeof(failed));
    memset(&failedSecondRound, '\0', sizeof(failedSecondRound));
    memset(&success, '\0', sizeof(success));
    memset(&successSecondRound, '\0', sizeof(successSecondRound));
    
    int for_i = 0;
    for(for_i = 1; for_i < argc; ++for_i) {
        // In every iteration we do the same thing that has happened in official-zh.c

        // Lets create the child task:
        pid_t child; // I created this variable here on purpose.

        if((child = fork())== -1){
        perror("Fork failed.");
        exit(EXIT_FAILURE); 
        }

        // Let's distinguish between parent and child tasks.
        if (child > 0) { // PARENT PROCESS!!!!!!
            // Blocking signals:
            sigprocmask(SIG_BLOCK, &signalSet, NULL);

            // so after we have our child we can send it the data it needs. read blocks, so we have time. :)
            printf("2 mp múlva küldöm az adatokat, a readnak blockolnia kell! \n");
                // Make some random task:
            char *task = "Here your task is.";
            int nthNum = for_i; // nth number of participant. It is gonna be a fixed 1 for now.
            sleep(2);

            // Lets attach number to the string:
            char num[12];
            memset(&num, '\0', sizeof(num));

            // Reading number with a string buffer kinda thing,
            // so the num variable I can use for concatenation in the next step.
            snprintf(num, sizeof(num), "%d", nthNum);

            // but first, let me take a selfie... no. Let's concatenate task to the string
            strcat(theMessenger, task);
            // Then concatenate name of the participant
            strcat(theMessenger, " ");
            strcat(theMessenger, argv[for_i]);
            // aaand the nthnum right after
            strcat(theMessenger, " ");
            strcat(theMessenger, num);

            write(pipefd[1], theMessenger, sizeof(theMessenger));

            // Data has been written, lets flush theMessenger!
            memset(&theMessenger, '\0', sizeof(theMessenger));        

            // waiting for child's answer
            // about this time we might get a signal from the child, so let's get ready for it!
            sigprocmask(SIG_UNBLOCK, &signalSet, NULL);

            printf("PARENT: Waiting for child's answer... \n ");
            pause();

            read(pipefd[0], theMessenger, sizeof(theMessenger)); // It would also block, but I have to use signals.

            // got this from the child:
            printf("PARENT: child says: %s \n", theMessenger);

            if(0 != strcmp(theMessenger, "1")) {
                // If it failed, save its name
                if(strlen(failed) > 0) {
                    strcat(failed, " ");
                }

                strcat(failed, argv[for_i]);
            } else {
                // Again, I am going for the grade...
                if(strlen(success) > 0) {
                    strcat(success, " ");
                }

                strcat(success, argv[for_i]);
            }

            printf("PARENT: Waiting for the child... \n");

            int status;
            waitpid(child, &status, 0); // We will wait for this process to end, need to keep things in sync.
            printf("Parent end. \n");
        } else {
            // CHILD PROCESS!!!!!         
            printf("CHILD: listening... \n");
            read(pipefd[0], theMessenger, sizeof(theMessenger));
            // theMessenger[BUFSIZ] = '\0';

            printf("CHILD: Data I got: %s \n", theMessenger);

            // This way we got everything we need.
            srand(time(NULL)); //the starting value of random number generation
            int randNum = rand() % 100; //number between 0-99
            
            memset(&theMessenger, '\0', sizeof(theMessenger));
            if (randNum > 50) {
                // Then we mark it as a successful try.
                strcat(theMessenger, "1");
                printf("CHILD: Sikersen megoldottam a feladatot. \n");
            } else {
                strcat(theMessenger, "0");
                printf("CHILD: Nem tudtam megoldani a feladatot. \n");
            }

            // Ok, we need to send a signal for the parent about the successness of the task.
            // Parent is ready for this ofc, a specific signal like a champ!
            // First, lets write the write side of the pipe, then send the signal alerting the parent about 
            // that it is actually time for reading.
            
            write(pipefd[1], theMessenger, sizeof(theMessenger));

            // Then send the signal 
            kill(getppid(), SIGUSR1); // we are done here

            printf("CHILD: END. \n");      

            // In the child, we are kinda in the middle of a for cycle, so lets break this A$AP
            break;
        }
    }

    // Probably this could have been done in a much clever way, but idc, going for grades!
    // Re-run things for failed members:
    fflush(NULL); // let's just flush those buffers first...

    // This is going to be ugly a bit but... Who cares now
    int iamparent = 0;

    if(strlen(failed) > 0) {
        char **fNames = explode(failed, &numParts);

        int j;
        for(j=0; j < numParts; ++j) {
            printf("Bukott: %s \n", fNames[j]);
            
            pid_t newChild;
            if(-1 == (newChild = fork())) {
                perror("Sikertelen pipe a második részben!");
                exit(EXIT_FAILURE);
            }

            if(newChild > 0) {
                // PARENT
                iamparent = 1;
                 sigprocmask(SIG_BLOCK, &signalSet, NULL);
                // Resend things like we did before:
                memset(&theMessenger, '\0', sizeof(theMessenger));

                strcat(theMessenger, "Az új feladatod.");
                strcat(theMessenger, " ");
                strcat(theMessenger, fNames[j]);
                strcat(theMessenger, " ");

                // Lets attach number to the string:
                char num[12];
                memset(&num, '\0', sizeof(num));

                // Reading number with a string buffer kinda thing,
                // so the num variable I can use for concatenation in the next step.
                snprintf(num, sizeof(num), "%d", (j+1));

                strcat(theMessenger, num);

                write(pipefd[1], theMessenger, sizeof(theMessenger));

                sigprocmask(SIG_UNBLOCK, &signalSet, NULL);

                printf("PARENT: Bukott child answerre várás... \n ");
                pause();

                read(pipefd[0], theMessenger, sizeof(theMessenger)); // It would also block, but I have to use signals.

                if(0 != strcmp(theMessenger, "1")) {
                    // If it failed, save its name
                    if(strlen(failedSecondRound) > 0) {
                        strcat(failedSecondRound, " ");
                    }

                    strcat(failedSecondRound, argv[j]);
                } else {
                    // Again, I am going for the grade...
                    if(strlen(successSecondRound) > 0) {
                        strcat(successSecondRound, " ");
                    }

                    strcat(successSecondRound, argv[for_i]);
                }

                int stat = 0;
                waitpid(newChild, &stat, 0); // We will wait for this process to end, need to keep things in sync.
            } else {
                // CHILD
                printf("CHILD: listening... \n");
                read(pipefd[0], theMessenger, sizeof(theMessenger));
                // theMessenger[BUFSIZ] = '\0';

                printf("CHILD: Data I got: %s \n", theMessenger);

                // This way we got everything we need.
                srand(time(NULL)); //the starting value of random number generation
                int randNum = rand() % 100; //number between 0-99
                
                memset(&theMessenger, '\0', sizeof(theMessenger));

                if (randNum > 19) { // 80%
                    // Then we mark it as a successful try.
                    strcat(theMessenger, "1");
                    printf("CHILD: Sikersen megoldottam a feladatot. \n");
                } else {
                    strcat(theMessenger, "0");
                    printf("CHILD: Nem tudtam megoldani a feladatot. \n");
                }
                
                write(pipefd[1], theMessenger, sizeof(theMessenger));

                // Then send the signal 
                kill(getppid(), SIGUSR1); // we are done here

                printf("CHILD: END. \n");      

                // In the child, we are kinda in the middle of a for cycle, so lets break this A$AP
                break;
            }
        }
    }    

    close(pipefd[0]);
    close(pipefd[1]);

    if (1 == iamparent) {
        printf("-------------------------------------- \n");
        printf("Összesített eredmény: \n");
        
        int l = 0;

        if(strlen(success) > 0) {
            printf("Első körben átmentek: \n");
            char **winFirst = explode(success, &numParts);
            
            for(l = 0; l < numParts; ++l) {
                printf("%s \n", winFirst[l]);
            }
        }

        if(strlen(failed) > 0) {
            printf("Első körben bukottak: \n");
            char **lostFirst = explode(failed, &numParts);
            
            for(l = 0; l < numParts; ++l) {
                printf("%s \n", lostFirst[l]);
            }
        }

        if(strlen(successSecondRound) > 0) {
            printf("Második körben győztek: \n");
            char **wonSec = explode(successSecondRound, &numParts);
            
            for(l = 0; l < numParts; ++l) {
                printf("%s \n", wonSec[l]);
            }
        }

        if(strlen(failedSecondRound) > 0) {
            printf("Második körben bukottak: \n");
            char **lostSec = explode(failedSecondRound, &numParts);
            
            for(l = 0; l < numParts; ++l) {
                printf("%s \n", lostSec[l]);
            }
        }
        
            
    }

	return 0;
}