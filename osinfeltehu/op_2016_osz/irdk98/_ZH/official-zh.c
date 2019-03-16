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
	// int numParts; // For my explode function

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
        int nthNum = 1; // nth number of participant. It is gonna be a fixed 1 for now.
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
        strcat(theMessenger, argv[1]);
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

        // NEXT PART IS OVERKILL (YET).
        // Parent sends data in one string, that is a nice way I think.
        // Let's deal with these things, lets "explode" the string into parts. 
        // char** stringParts = explode(theMessenger, &numParts);
        // my version of this explode function alerts a variable through its pointer that
        // tells you very qucikly how many pieces it could break your string into. (numParts)

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
    }

    close(pipefd[0]);
    close(pipefd[1]);

	return 0;
}