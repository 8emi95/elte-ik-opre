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
	DIR* FD;
	struct dirent* in_file;
	FILE    *entry_file;
	int numParts; // For my explode function

	char fullEventName[BUFSIZ];
	char    buffer[BUFSIZ];
	char pipeMessager[BUFSIZ]; // This I will use for messaging throuh my pipes.

	char *in_dir = "events/";
	char **explodedLine;
	int pipefd[2]; // For the pipe

	// Child process can send signals too early, so lets block 'em for a while:
	sigset_t sigset;
	sigemptyset(&sigset); // empty signal set
	sigaddset(&sigset, SIGUSR1); // SIGUSR1 is in set

	signal(SIGUSR1, signalHandler);

	/* Scanning the in directory */
	if (NULL == (FD = opendir (in_dir)))
	{
		fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));

		return 1;
	}

	int numChild = 0;

	while ((in_file = readdir(FD)))
	{
		// I want to child process to do no such thing ad file reading, or whatever, so I will put that code here.
		/* Skipping current & parent dirs like: */
		if (!strcmp (in_file->d_name, "."))
			continue;
		if (!strcmp (in_file->d_name, ".."))
			continue;

		// Lets open the pipe:
		if (pipe(pipefd) == -1)
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}

		// Making this block BEFORE the fork, so everytime a child process arises, parent is gonna be
		// ready for its signal.
		sigprocmask(SIG_BLOCK, &sigset, NULL); // signals in sigset will be blocked
		// parameters, how: SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK -   ;
		// 2. parameter changes the signalset to this if it is not NULL,
		// 3.parameter if it is not NULL, the formerly used set is stored here

		// after getting members from the events lets try a fork call
		pid_t child; // Let's try not causing so many errors from here... :D
		if ((child = fork()) < 0) {
			perror("Bad fork call.");
		}

		// It would be great to know what I am doing. No problem, by the end of this code I am gonna be a masssteeeerrrr.
		// https://cdn-images-1.medium.com/max/455/1*snTXFElFuQLSFDnvZKJ6IA.png

		if (child > 0) {
			// PARENT PROCESS

			/* Open directory entry file for common operation */
			// Setting the right path of the file:
			memset(&pipeMessager, '\0', sizeof(pipeMessager));
			memset(&fullEventName, '\0', sizeof(fullEventName)); // Make it full of 0.
			strcat(fullEventName, in_dir);
			strcat(fullEventName, in_file->d_name);

			entry_file = fopen(fullEventName, "r");
			if (entry_file == NULL)
			{
				fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));

				return 1;
			}

			/* Doing some struf with entry_file : */
			/* For example use fgets */
			while (fgets(buffer, BUFSIZ, entry_file) != NULL)
			{
				// printf("%s \n", buffer);
				// TODO: Add protection for empty files!
				if (strlen(buffer) > 0) { // this does not work :(
					explodedLine = explode(buffer, &numParts);

					if (strlen(pipeMessager) > 0) {
						strcat(pipeMessager, " ");
						strcat(pipeMessager, explodedLine[0]);
					} else {
						// Frist insertion
						strcat(pipeMessager, explodedLine[0]);
					}
				}

			}

			/* When you finish with the file, close it */
			fclose(entry_file);

			// Frist, we have to send some dummy info to the child process which acts as a location of the event.
			printf("Parent: 2 masodperc mulva kuldom a helyszínt a gyerek folyamatnak... \n");
			fflush(NULL);
			sleep(2);

			// To see whats going on I am sending numbers to my child processes. So far so good.
			++numChild;
			char temp[150];
			char szam[12];
			memset(szam, '\0', sizeof(szam));
			memset(temp, '\0', sizeof(temp));
			snprintf(szam, 12, "%d", numChild);
			strcat(temp, szam);
			strcat(temp, ". gyerek folyamat vagy!");
			// End of "fun" part.

			// using temp instead of pipmessenger, since that contains names.
			fflush(NULL);  // flushes all write buffers (not necessary)
			write(pipefd[1], temp, sizeof(temp));

			printf("PARENT: Szulo beirta az adatokat a csobe! \n");
			printf("PARENT: Szulo var a signalra a gyerektol! \n");

			// I'll unblock here the SIGUSR1 (NOTE: blocking in child is active, but no one cares :P)
			sigprocmask(SIG_UNBLOCK, &sigset, NULL);
			// So basically after fork we just wait in the parent till a signal arrives from the child. Well okay mate.
			pause(); // waits till a signal arrives
			sigprocmask(SIG_BLOCK, &sigset, NULL); // Let's wait for the child to give a sign that it has finished writing to the buff

			// Sending names of people attending the event:
			printf("PARENT: Résztvevők küldése a gyereknek... \n");
			write(pipefd[1], pipeMessager, sizeof(pipeMessager));
			printf("PARENT: Mennyire volt sikeres a rendezveny? \n");

			sigprocmask(SIG_UNBLOCK, &sigset, NULL); // NOTE: find a better way to do this...
			pause();
			sigprocmask(SIG_BLOCK, &sigset, NULL); // Instant signal blocking after we got that, since we have stuff to do!

			// We are good to read now...
			memset(pipeMessager, '\0', sizeof(pipeMessager));

			read(pipefd[0], pipeMessager, sizeof(pipeMessager)); // blocking!
			printf("PARENT: Sikeresség: %s%% \n", pipeMessager);

			// Another reading started... (Waiting for those, who have not attended.)
			memset(pipeMessager, '\0', sizeof(pipeMessager));

			// Sending signal to the child signing that we are ready for reading.
			kill(child, SIGUSR1);

			sigprocmask(SIG_UNBLOCK, &sigset, NULL);
			pause(); // Don't read till it could re-read data...
			read(pipefd[0], pipeMessager, sizeof(pipeMessager)); // can block -> Ken Block lol.

			if (strlen(pipeMessager) > 0) {
				printf("PARENT: Nem vettek részt: %s \n", pipeMessager);
			} else {
				printf("PARENT: Mindenki megjelent. \n");
			}

			printf("PARENT: Várok a gyerekre, hogy befejeződjön... \n");
			int status;
			waitpid(child, &status, 0); // We will wait for this process to end, need to keep things in sync.

			// Closing this pipe.
			close(pipefd[1]);
			close(pipefd[0]);
			fflush(NULL);
			printf("PARENT: Parent process while part ended (and so its child!). \n");
			printf("------------------------------------------------------- \n");
		} else {
			// CHILD PROCESS
			// Getting ready to read the pipe:
			sigprocmask(SIG_BLOCK, &sigset, NULL); // This signal I am blocking!

			printf("CHILD: Child (%d): gyerek folyamat válaszra vár a pipetol... \n", getpid());
			read(pipefd[0], pipeMessager, sizeof(pipeMessager)); // If my assumptions are right, then read is blocking, so we'll wait till we get smthg to read.
			fflush(NULL); // write buffers flushed.

			// Lets print this:
			printf("CHILD: message from the pipe: %s \n", pipeMessager);

			// Send that long-awaited singal to the parent:
			kill(getppid(), SIGUSR1); // I am blocking this signal just to make sure I can catch it!
			printf("CHILD: Child process sent signal to (%d) \n", getppid());

			// Lets clear our buffer
			memset(pipeMessager, '\0', sizeof(pipeMessager));
			read(pipefd[0], pipeMessager, sizeof(pipeMessager)); // This would make this process wait anyway... (good thing!)
			fflush(NULL);

			printf("CHILD: Resztvevok: %s \n", pipeMessager);

			// Generate how successful the event was:
			srand(time(NULL)); //the starting value of random number generation
			int successRate = rand() % 100 + 1; //number between 0-99

			// handling int -> string conversion, etc...
			char szam[12];
			memset(szam, '\0', sizeof(szam));
			snprintf(szam, 12, "%d", successRate);

			write(pipefd[1], szam, sizeof(szam));
			kill(getppid(), SIGUSR1);

			sigprocmask(SIG_UNBLOCK, &sigset, NULL); 
			pause(); // Waiting for the parent to tell us we can send remainig data.

			// Now parent is waiting for us to determine who has arrived, an who not.
			// This is gonna be done pretty easily, and with help of some very very basic mathematics.
			// Suppose we can get random numbers from [0-99] with the equal possibility of 1/100.
			// Let's say we mark a scenario "good", if our number is less or equal to 9. Since this has
			// a possiblity of 1/100 for each individual number, because of the or it adds up to 10/100,
			// which result in a possibility rate of p = 0,1 = 10%.
			// Lets apply this to every name we got in the list, ofc, we have to explode first.
			// BE AWARE, WE ARE MOSTLY WORKING WITH CHAR POINTERS, SO DO NOT USE WHAT YOU ARE EXPLODING!!

			char * copy = malloc(strlen(pipeMessager) + 1);
			strcpy(copy, pipeMessager);

			char **names = explode(copy, &numParts);
			// Since this very very nice explode function tells you
			// how many pieces it has made, I can easily iterate over those. Otherwise it would be a pain
			// in the ass, or even impossible to count how many elements there are in the array.
			// (maybe by using strtok and counting whitespaces I could achieve something... This time I
			// prefer this way, I would loved to use pointers anyway.)

			memset(pipeMessager, '\0', sizeof(pipeMessager));

			int k;
			for (k = 0; k < numParts; ++k) {
				// Implementing the logic I described above:
				int randNum = rand() % 100; //number between 0-99
				// printf("%s - %d \n", names[k], randNum);
				// printf("names[%d] : %s \n", k, names[k]);

				// "Every attender might not arrive by the possibility of 10%."
				// So they have 90% to arrive. GGWP HARDCORE MATHEMATICS
				if (randNum >= 90) { // 0-89 : 90 numbers :) ==> 90-99: 10 numbers. (10%)
					// So if someone does not arrive... (logic like this has been used above)
					// pipeMessager is now used to store names of those who have not arrived.
					if (strlen(pipeMessager) > 0) { // strlen does not counts terminating zeros! :)
						strcat(pipeMessager, " ");
						strcat(pipeMessager, names[k]);
					} else {
						// Frist insertion
						strcat(pipeMessager, names[k]);
					}
				}
			}

			// Let send back who has not arrived:
			write(pipefd[1], pipeMessager, sizeof(pipeMessager));
			kill(getppid(), SIGUSR1);

			free(copy); // Do not forget our copy :)
			// Closing the pipe (NOTE: child should never ever have a chance to get into another loop!)
			close(pipefd[1]);
			close(pipefd[0]);
			fflush(NULL); // flushing the pipes.
			// Oh boi, we are in the middle of a while cycle in the child! Lets end this:
			break;
		}
	}

	return 0;
}