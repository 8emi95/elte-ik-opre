#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <unistd.h>

void handler1(int signumber){	//T1
  printf("Tarsasag 1 megerkezett.\n",signumber);
}

void handler2(int signumber){	//T2
  printf("Tarsasag 2 megerkezett.\n",signumber);
}

void handler3(int signumber){	//T2
  printf("Kuldom a pincert.\n",signumber);
}

int main(){
	int message = 0;  // int for reading from pipe
	int message2 = 0; 
	pid_t t1, t2;
	
	int pipefd[2];
	int pipefd2[2];
	
	signal(SIGTERM, handler1); //T1
	signal(SIGUSR1, handler2); //T2
	signal(SIGILL, handler3);
	
	pipe(pipefd);
	pipe(pipefd2);
	
	t1 = fork();
	if (t1 == 0) {
		//////////////////////
		//    t1 kód        //
		//////////////////////
		kill(getppid(),SIGUSR1);	//vendég megérkezett
		
		sleep(3);
		
		
		//READING FROM THE PIPE
		close(pipefd2[1]);  // Close the unused write descriptor
		close(pipefd[1]);  // Close the unused write descriptor
		close(pipefd[0]); // close the used read descriptor
		
		printf("T1 reads the Table Number 2 from the pipe!\n");
		read(pipefd2[0], &message2, sizeof(message2)); // reading 1 integer from the pipe
		printf("T1 has read the Table Number 2 from Fonok: %i\n", message2);
		
		close(pipefd2[0]); // close the used read descriptor
		
		kill(getppid(), SIGILL);
		
		
		sleep(2);
		
		printf("Tarsasag 1 tavozik.\n");
	} else {
		t2 = fork();

		if (t2 == 0) {
			//////////////////////
			//    t2 kód        //
			//////////////////////
			sleep(1);
			kill(getppid(), SIGTERM);	//vendég megérkezett
			
			sleep(3);
			
			//READING FROM THE PIPE
			close(pipefd[1]);  // Close the unused write descriptor
			close(pipefd2[1]);  // Close the unused write descriptor
			close(pipefd2[0]); // close the used read descriptor
			
			printf("T2 reads the Table Number from the pipe!\n");
			read(pipefd[0], &message, sizeof(message)); // reading 1 integer from the pipe
			printf("T2 has read the Table Number from Fonok: %i\n", message);
			
			close(pipefd[0]); // close the used read descriptor
			
			
			
			kill(getppid(), SIGILL);
			
			sleep(3);
			
			printf("Tarsasag 2 tavozik.\n");
		} else {
			//////////////////////
			//    fonok kód     //
			//////////////////////
			sleep(2);
			pause();
			printf("A vendegek megerkeztek.\n");
			printf("Fonok: Udvozoljuk onoket. Kerem uljenek le az asztaljukhoz.\n");
			
			//WRITING IN THE PIPE
			close(pipefd[0]); //close the read descriptor
			close(pipefd2[0]); //close the read descriptor
			
			srand(time(NULL));
			int tableNum = (rand() % 5) + 1;
			int tableNum2 = ((tableNum + 1)%5) +1;
			
			printf("Table Number: %i\n", tableNum);
			printf("Table Number2: %i\n", tableNum2);
			write(pipefd2[1], &tableNum, sizeof(tableNum));
			write(pipefd[1], &tableNum2, sizeof(tableNum2));
			close(pipefd[1]); // Closing write descriptor
			close(pipefd2[1]); // Closing write descriptor
			printf("Fonok has written the data in the pipe!\n");
			fflush(NULL);
			
			
			pause();
			
			
			
			
			wait();  //a főnök vár, amíg elmennek a vendégek
		}
		wait();
	}
}	