#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
 
const char* requestPipeFile = "request";
const char* answerPipeFile = "answer";
const char guests[12][128] = {
	"Horvath Tibor",
	"Kis Zoltan",
	"Puma Nandor",
	"Safrany Emese",
	"Sarka Kata",
	"Nagy Aladar",
	"Kerekes Jozsef",
	"Lakatos Levente",
	"Simon Maya",
	"Takacs Dominik",
	"Jedlik Anyos",
	"Deak Ferenc"
};

const char event[2][128] = {
	"Nyiregyhaza",
	"Zuglo"
};
 
int unlinkPipes(){
	unlink(requestPipeFile);
	unlink(answerPipeFile);
	
	return 0;
}

void handler(int signumber){
  printf("A partner megérkezett a helyszínre.\n");
}

void printMissingGuests(const char * processOwner, const char guests[][128]) {
	printf("%s: Az alabbi vendegek nem erkeztek meg a rendezvenyre:\n", processOwner);
	int i;
	for (i = 0; i < 12; ++i){
		srand(time(NULL) + rand());
		int random = rand() % 10;
		if(random == 1) {
			printf("%s", guests[i]);
		}
	}
}

void sendEventLocation(int request, const char event[][128], int c) {
	write(request, event[c], strlen(event[c]) + 1);
}

void printSuccess(int answer, const char * processOwner) {
	int sucess;
	read(answer, &sucess, sizeof(sucess));
	printf("%s: a rendezveny sikere: %d\n", processOwner, sucess);
}

void printGuestList(int request, const char * processOwner) {
	char guestList[12][128];
	read(request, guestList, sizeof(guestList));
	
	int i;
	for (i = 0; i < 12; ++i){
		printf("%s: %s\n", processOwner, guestList[i]);
	}
}


int main(int argc, char * argv[]) {
	int eventArrayLength = sizeof(event) / sizeof(event[0]);
	
	struct sigaction sigact;
	sigact.sa_handler = handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0; 
	sigaction(SIGUSR1,&sigact,NULL);
	
	srand(time(NULL));

	int childProcessId, request, answer;
	int c = 0;

	mkfifo(answerPipeFile, S_IRUSR | S_IWUSR);
	mkfifo(requestPipeFile, S_IRUSR | S_IWUSR);
	
	childProcessId = fork();
	char * processOwner;
	
	if (childProcessId > 0) {
		processOwner = "Kozpont";
		while (c != eventArrayLength){
			request = open(requestPipeFile, O_WRONLY);
			answer = open(answerPipeFile, O_RDONLY);
			
			sendEventLocation(request, event, c);
			pause();
			
			write(request, guests, sizeof(guests));

			printSuccess(answer, processOwner);
			printMissingGuests(processOwner, guests);

			printf(" \n\n");
			close(request);
			close(answer);
			c++;
		}
	
		unlinkPipes();
	} else {
		processOwner = "Partner";
		char receiveMessage[1024];
		char sendMsg[1024];
		
		while (c != eventArrayLength){
			request = open(requestPipeFile, O_RDONLY);
			answer = open(answerPipeFile, O_WRONLY);
			
			read(request, receiveMessage, sizeof(receiveMessage));
			printf("%s: Utazas a helyszínre... (%s)\n", processOwner, receiveMessage);
			kill(getppid(),SIGUSR1);
			
			printGuestList(request, processOwner);

			int random = rand() % 10;			
			write(answer, &random, sizeof(random));
			
			close(request);
			close(answer);
			c++;
		}

	}

	return 0;
}