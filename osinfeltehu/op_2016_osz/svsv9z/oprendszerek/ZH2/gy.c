#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h>
#include <string.h>
#include <errno.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

void sighandler(int signum) {
	printf("CAUGHT SIGNAL!\n", signum);
}

void waitingForVisitors(int signum) {
}

void visitorsArrived(int signum) {
	printf("Megerkeztek a vendegek!\n");
}

void showEndedSignal(int signum) {
	printf("A rendezveny lezajlott! Sikeressege: %i\n", rand()%100);
}

int main( int argc, const char* argv[] )
{
	srand(getpid());
	char *fruit[] = {
		"Hagymasi Daniel", "Dani",
        "Daniel", "Hagymasi Dani"
    };

	
	int status;
	char string[] = "Hello, world!\n";
	int fd[2], nbytes;
	int pfd[2];
	char readbuffer[80];
	char buf[1000];
	
	int lengthPipe[2];
	
	int nameLengths[100];
	
	
    pipe(fd);
	pipe(pfd);
	pipe(lengthPipe);
        
	
	pid_t child = fork();
	
	if (child < 0) {
		perror("The fork calling was not succesful\n"); exit(1);
	} 
	
	if (child > 0) { //parent process
			printf("PARENT KESZ");
	
		signal(SIGUSR1, sighandler);
		pause();

		close(fd[0]);
		
		write(fd[1], string, (strlen(string)+1));
		pause();
		//printf("Received signal!\n");
		
		int i = 0;
		
		int num_fruit = 4;

        for (; i < num_fruit; i++) {
			close(pfd[0]);
            //printf("Current fruit: %s\n", fruit[i]);
            write(pfd[1], fruit[i], (strlen(fruit[i])));
			nameLengths[i] = strlen(fruit[i]);
			//printf("length of %s : %i ", fruit[i] , nameLengths[i]);
        }

		i = 0;
		for (; i < num_fruit; i++) {
			close(lengthPipe[0]);
           // printf("Curr length: %i\n", nameLengths[i]);
            write(lengthPipe[1], &nameLengths[i], sizeof(nameLengths[i]));

		}

		kill(child, SIGUSR1);
		

		signal(SIGUSR1, showEndedSignal);

		pause();
		//wait(&status);

		
		//waitpid(child,&status,0);
		
	}
	else { //child process
		printf("1. KESZ");
		kill(getppid(), SIGUSR1);

		close(fd[1]);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        /*printf("Received string: %s", readbuffer);*/
		kill(getppid(), SIGUSR1);
		
		close(fd[0]);
		int i = 0;
		
		signal(SIGUSR1, sighandler);
		pause();

		int bytes = read(pfd[0], buf, sizeof(buf));
			

			
		//printf("Bytes: %i", bytes);

		/*printf("Fruit Fetched: \n");
		for (; i < bytes; ++i) {
			printf("%c", buf[i]);
		}*/
		//printf("Fruit Fetched: %s\n", buf);
		

		char names[5000];
		
		i = 0;
		int c = 0;
		int counter = 0;
		int currByte = 0;

		int nLengths[100];
		int didntArrive[100];
	


		for (; i < 4; i++) {
			read(lengthPipe[0], &nLengths[i], sizeof(nLengths[0]));
			//printf("nLengths[%i] = %i", i, nLengths[i]);
		}

		i = 0;
		c = 0;
		currByte = 0;
		counter = 0;


	
		printf("\nA vendegek nevei: \n ");
		for (; c < 4; c++) {
	
			counter += nLengths[c];
			for (i = currByte; i < counter ; i++) {
				printf("%c", buf[i]);
				names[i] = buf[i]; 
			}
			currByte = counter;
			printf("\n ");
		}



		printf("\nVarom a vendegeket...\n");
		//kill(getppid(), SIGUSR1);

		sleep(0.1f);

		printf("Rendezveny lebonyolitasa...\n");
		sleep(0.1f);


		

		i = 0;
		int numOfDidntArrive = 0;
		char namesOfDidntArrive[1000];
		for (; i < 4; i++) {
			int chance = rand()%100;
			if (chance < 10) {
				didntArrive[i] = 1;
				numOfDidntArrive++;
			}
			else {
				didntArrive[i] = 0;
			}
		}

		
		if (numOfDidntArrive != 0) {
		printf("\nAkik nem erkeztek meg: \n");
		}
		
		i = 0;
		int currInd = 0;
		int ind = 0;
		
		int namesInd = 0;
		for(; i < 4; i++) {
			if (i != 0) {
				currInd = currInd + nLengths[i - 1];
			}
			
			if (didntArrive[i] == 1) {
				printf(" ");
				for (ind = currInd; ind < currInd + nLengths[i]; ind++) {
					namesOfDidntArrive[namesInd] = names[ind];
					printf("%c", namesOfDidntArrive[namesInd]);
					namesInd++;
				}
				printf("\n");
				
			}
		}
		
		
		
		/*printf("\nAkik nem erkeztek meg: \n");
		curr = 0;
		for (; curr < numOfDidntArrive; curr++) {
			for (i = 0; i < ; i++) {
				printf("%c", namesOfDidntArrive[i]);
			}
		}*/
		//sendM(0);
		
		kill(getppid(), SIGUSR1);
		
	}
}