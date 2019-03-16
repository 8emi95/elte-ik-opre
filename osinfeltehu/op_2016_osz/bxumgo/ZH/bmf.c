#include <signal.h> //signalok
#include <stdio.h>	//Fájl kezelés + kiiratás
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

void	BMF();
void	KeszJel();
void	Szerelveny();

int szerelvenypipe[2];
int osztmemID;
key_t kulcs;
char *osztott;

int main(int argc, char** argv)
{
	printf("\n");
	
	if (argc <= 1)	//Filtering the uncommanded executions.
	{
		printf("Nincs szerelveny szam.\n\n");
		return 1;
	}
	
	int max = atoi(argv[1]);
	if (max <= 0)
	{
		printf("Nem megfelelo szam!\n\n");
		return 1;
	}
	
	pipe(szerelvenypipe);
	kulcs = ftok(argv[0],1);
	osztmemID = shmget(kulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
	osztott = shmat(osztmemID, NULL, 0);
	
	BMF(max);
}

void BMF(int nSzerelvenyek)
{
	int szerelveny[nSzerelvenyek];
	//write(szerelvenypipe[1], &n, sizeof(int));
	//write(szerelvenypipe[1], &nSzerelvenyek, sizeof(int));
	
	struct sigaction sigact;
	sigact.sa_handler = KeszJel;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGUSR1, &sigact, NULL);
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGUSR1);
	//sigsuspend(&sigset);
	
	int i, nPot = 0;
	for (i = 0; i < nSzerelvenyek; ++i)
	{
		szerelveny[i] = fork();
		if (szerelveny[i] == 0)
		{
			Szerelveny();
			return;
		}
		
		int ezPot = 0;
		write(szerelvenypipe[1], &ezPot, sizeof(int));
		write(szerelvenypipe[1], &i, sizeof(int));
		write(szerelvenypipe[1], &nSzerelvenyek, sizeof(int));
		sigsuspend(&sigset);
		
		int sorszam, siker;
		read(szerelvenypipe[0], &sorszam, sizeof(int));
		read(szerelvenypipe[0], &siker, sizeof(int));
		printf("BMF: %d. szerelveny uzeni: %s\n", sorszam + 1, siker ? "Sikerült." : "Bananhej.");
		printf("BMF: szerelveny #%s\n", osztott);
		
		if (siker)
			continue;
		
		nPot++;
		szerelveny[i] = fork();
		if (szerelveny[i] == 0)
		{
			Szerelveny();
			return;
		}
		
		ezPot = 1;
		write(szerelvenypipe[1], &ezPot, sizeof(int));
		write(szerelvenypipe[1], &i, sizeof(int));
		write(szerelvenypipe[1], &nSzerelvenyek, sizeof(int));
		sigsuspend(&sigset);
		
		read(szerelvenypipe[0], &sorszam, sizeof(int));
		read(szerelvenypipe[0], &siker, sizeof(int));
		printf("BMF: %d. szerelveny uzeni: %s\n", sorszam + 1, siker ? "Sikerült." : "Bananhej.");
		printf("BMF: szerelveny #%s\n", osztott);
	}
	
	int status;
	for (i = 0; i < nSzerelvenyek + nPot; ++i)
		wait(&status);
	
	shmdt(osztott);
    shmctl(osztmemID,IPC_RMID,NULL);
	
	printf("BMF: Potjaratok szama: %d\n\n", nPot);
}

void KeszJel(int signumber)
{
	
}

void Szerelveny()
{	
	//sleep(1);

	srand(getpid());
	int sorszam, max, siker, ezPot;
	read(szerelvenypipe[0], &ezPot, sizeof(int));
	read(szerelvenypipe[0], &sorszam, sizeof(int));
	read(szerelvenypipe[0], &max, sizeof(int));
	siker = rand() % 10;
	printf("%d. %sszerelvény: Utanam még %d. %s\n", sorszam + 1, ezPot ? "pot" : "", max - sorszam - 1, siker ? "Siker." : "Bananhej.");
	
	char buffer[64];
	sprintf(buffer, "%d, utazott: %d", sorszam + 1, siker ? (rand() % 21 + 10) : -1);
	
    strcpy(osztott,buffer);
	
	kill(getppid(), SIGUSR1);
	/*char uzenet[32];
	if(siker)
		memccpy(uzenet, "Sikerult", sizeof(char) * 8);
	else
		memccpy(uzenet, "Bananhej", sizeof(char) * 8);*/
	write(szerelvenypipe[1], &sorszam, sizeof(int));
	write(szerelvenypipe[1], &siker, sizeof(int));
	
	shmdt(osztott);
}