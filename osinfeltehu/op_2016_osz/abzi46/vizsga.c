//Balog Balázs, ABZI46
#include <stdio.h>
#include <stdlib.h> //exit(1)
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void signalHandler(int);
int signalReceived = 0;

void signalHandler(int sig){
		if(sig == SIGUSR1){
			//printf("Signal caught\n");
			signalReceived = 1;
		}
}

void jatekos(char* myfifo){
	char nev[50];
	char feladat[50];
	int sorszam;
	int fe;
	fe=open(myfifo, O_RDONLY);
	read(fe, &sorszam, sizeof(sorszam));
	read(fe, nev, sizeof(nev));
	read(fe, feladat, sizeof(feladat));
	close(fe);
	printf("%i. %s: %s\n", sorszam, nev, feladat);
	time_t t;
	srand((unsigned) time(&t));
	int siker = (rand() % 1);
	char uzenet[50];
	if(siker = 1){
		sprintf(uzenet,"A feladat megoldása sikeres!\n");
	}
	else{
		sprintf(uzenet,"A feladat megoldása sikertelen!\n");
	}
	fe=open(myfifo, O_WRONLY);
	write(fe, uzenet, sizeof(uzenet));
	close(fe);
	
	kill(getppid(),SIGUSR1);
	exit(0);
}

int main(int argc,char ** argv){

if(argc < 2){
	printf("argumentumba írja a versenyző nevét!");
	exit(1);
}

pid_t pID = fork();

int fd;
char * myfifo = "myfifo";
mkfifo(myfifo, 0666);

if (pID == 0)                // child
   {
	   jatekos(myfifo);
    }
else if (pID < 0)            // failed to fork
    {
		printf("Failed to fork\n");
        exit(1);
    }
	//parent

signal(SIGUSR1, signalHandler);
int sorszam = 1;
char feladat[50] = "Szabadulj!";
char nev[50];
sprintf(nev, argv[1]);
fd=open(myfifo, O_WRONLY);
write(fd, &sorszam, sizeof(sorszam));
write(fd, nev, sizeof(nev));
write(fd, feladat, sizeof(feladat));
close(fd);

char uzenet[50];
fd=open(myfifo, O_RDONLY);
read(fd, uzenet, sizeof(uzenet));
close(fd);
unlink(myfifo);

while(signalReceived == 0);
printf(uzenet);
return 0;
}