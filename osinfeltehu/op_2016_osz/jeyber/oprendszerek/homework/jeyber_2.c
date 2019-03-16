#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber) {
  signal(SIGTERM,SIG_DFL);
}

void createPipeForEventLocation(int * pipeEventLocation) {
	if (pipe(pipeEventLocation) == -1) {
        perror("\nUnable to create pipe for event location!\n");
    }
}

void createFileForPipe(const char * pipeFileName) {
	int fileDescriptor;
	fileDescriptor = open(pipeFileName,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fileDescriptor < 0) {
		perror("\nUnable to open file!\n");
		exit(1);
	}
	close(fileDescriptor);
}

void fillFilleForPipe(const char * guestFileName, const char * pipeFileName, const char * eventId) {
	FILE * f;
	FILE * g;
	char t[1000];
	
	f = fopen(guestFileName,"a+");
	g = fopen(pipeFileName,"a+");
	
	while(fgets(t,sizeof(char)*1000,f))	{
		char * event;
		char * guestName;
		char * token;

		read(f,&t,sizeof(t));
		guestName = strtok(t, "}");
		strtok(NULL, "}");
		event = strtok(NULL, "}");
		strtok(NULL, "}");

		if(strcmp(eventId,event) == 0) {
			fputs(guestName,g);
			fputs("\n",g);
		}

	}
	fclose(f);
	fclose(g);
}

void requestEventOrganization(char * processOwner, const char * eventId, const int * pipeEventLocation, const char * eventLocation) {
	printf("%s: Requesting partner to organize event: %s\n", processOwner, eventId);
	close(pipeEventLocation[0]);
	write(pipeEventLocation[1],eventLocation,strlen(eventLocation));
	close(pipeEventLocation[1]);
}

void receiveGuestList(const char * processOwner, const char * pipeFileName, char * s, FILE * p) {
	printf("%s: Guestlist received. Guests:\n", processOwner);
	p = fopen(pipeFileName,"a+");
	while(fgets(s,sizeof(char)*1024,p))	{
		printf("%s", s);
	}  
	fclose(p);
}

void printMissingGuests(const char * processOwner, FILE * p, const char * pipeFileName, char * s) {
	printf("%s: Missing guests:\n", processOwner);
	int a = 0;
	p = fopen(pipeFileName,"a+");
	while(fgets(s,sizeof(char)*1024,p))	{
		a++;
		srand(time(NULL) + a);
		int r2 = rand() % 10;
		if(r2 == 1) {
			printf("%s", s);
		}
	}
	fclose(p);
}



int main() {
	char s[1024];
	
	char * eventId;
	char * eventName;
	char * eventLocation;
	
	FILE * events = fopen("event.txt","a+");
	fgets(s,sizeof(char)*1024,events);
	read(events,&s,sizeof(s));
	eventId = strtok(s, "}");
	eventName = strtok(NULL, "}");
	eventLocation = strtok(NULL, "}");
	fclose(events);
	
	const char * guestFileName = "guest.txt";
	const char * pipeFileName = "pipe.txt";
	
	int pipeEventLocation[2];
    char charsFromPipe[100];
	
	pid_t childProcessId;
	
	signal(SIGTERM,handler);
	
	createPipeForEventLocation(pipeEventLocation);
	createFileForPipe(pipeFileName);
	fillFilleForPipe(guestFileName, pipeFileName, eventId);	
	
	mkfifo(pipeFileName, S_IRUSR | S_IWUSR );
	
	system("clear");

	childProcessId = fork();
	char* processOwner;
	
	if(childProcessId > 0 )	{
		processOwner = "CENTER";
		
		requestEventOrganization(processOwner, eventName, pipeEventLocation, eventLocation);
		pause();
		
		printf("%s: Sending guest list to the organizer...\n", processOwner);
		sleep(1);
		kill(childProcessId,SIGTERM);
		
		int status;
		wait(&status);
		
		events = fopen("event.txt","a+");
		fgets(s,sizeof(char)*1024,events);
		fgets(s,sizeof(char)*1024,events);
		read(events,&s,sizeof(s));
		eventId = strtok(s, "}");
		eventName = strtok(NULL, "}");
		eventLocation = strtok(NULL, "}");
		fclose(events);
		
		printf(" \n%s: Next event: %s\n", processOwner, eventName);
	} else {
		processOwner = "PARTNER";
		FILE * p;
		
		close(pipeEventLocation[1]);
		sleep(1);
		read(pipeEventLocation[0],charsFromPipe,sizeof(charsFromPipe));
		printf("%s: Location received: %s\n", processOwner, charsFromPipe);	
		sleep(1);
				
		printf("%s: Arrived to %s waiting for the guests...\n", processOwner, charsFromPipe);
		sleep(1);
		kill(getppid(),SIGTERM);
		pause();
		
		receiveGuestList(processOwner, pipeFileName, s, p);
		sleep(1);
		
		srand(time(NULL));
		int random = rand() % 100;
		printf("%s: Event rate: %i %%\n", processOwner, random);
		sleep(1);
		
		printMissingGuests(processOwner, p, pipeFileName, s);
		
        unlink(pipeFileName);
	}
	
	return 0;
}