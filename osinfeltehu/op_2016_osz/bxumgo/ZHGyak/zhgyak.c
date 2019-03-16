#include <signal.h> //signalok
#include <stdio.h>	//Fájl kezelés + kiiratás

void	Elnok();
void	Ellenor();
void	KeszJel();
void	Kezelo();

int jelzesek = 0;
int ellenorpipe[2], kezelopipe[2];

int main(int argc, char** argv)
{
	printf("\n");
	
	int ellenor, kezelo;
	pipe(ellenorpipe);
	pipe(kezelopipe);
	
	ellenor = fork();
	if (ellenor > 0)
	{
		kezelo = fork();
		if (kezelo > 0)
			Elnok();
		else
			Kezelo();
	}
	else
		Ellenor();
	
	
}

void Elnok()
{
	close(ellenorpipe[0]);
	close(kezelopipe[0]);
	close(kezelopipe[1]);
	
	printf("%-7s: Itt vagyok!\n", "Elnok");
	
	struct sigaction sigact;
	sigact.sa_handler = KeszJel;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGUSR1, &sigact, NULL);
	sigaction(SIGUSR2, &sigact, NULL);
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGUSR1);
	sigdelset(&sigset, SIGUSR2);
	while (jelzesek != 3)
		sigsuspend(&sigset);
	
	char message[32];
	printf("%-7s: Szavazok szama: ", "Elnok");
	fgets(message, sizeof(message), stdin);
	int nSzavazok = atoi(message);
	write(ellenorpipe[1], &nSzavazok, sizeof(int));
	
	srand(getpid());
	int i;
	for(i = 0; i < nSzavazok; ++i)
	{
		int n = rand() % 1000;
		write(ellenorpipe[1], &n, sizeof(int));
	}
	close(ellenorpipe[1]);
	
	int status;
	wait(&status);
	wait(&status);
	printf("%-7s: Mindenki halott! :(\n\n", "Elnok");
}

void Ellenor()
{
	close(ellenorpipe[1]);
	close(kezelopipe[0]);
	close(kezelopipe[1]);
	
	sleep(1);
	printf("%-7s: Itt vagyok!\n", "Ellenor");
	kill(getppid(), SIGUSR1);
	int nSzavazok;
	read(ellenorpipe[0], &nSzavazok, sizeof(int));
	printf("%-7s: Varok %d szavazot\n", "Ellenor", nSzavazok);
	
	int i, szavazok[nSzavazok];
	for (i = 0; i < nSzavazok; ++i)
	{
		read(ellenorpipe[0], &szavazok[i], sizeof(int));
		printf("%-7s: %d. szavazo: %d\n", "Ellenor", i + 1, szavazok[i]);
	}
	close(ellenorpipe[0]);
}

void KeszJel(int signumber)
{
	if (signumber == SIGUSR1 && jelzesek % 2 == 0)
	{
		jelzesek += 1;
	printf("%-7s: Ellenor kesz!\n", "Elnok");
		
	}
	else if (signumber == SIGUSR2 && jelzesek % 4 - jelzesek % 2 == 0)
	{
		jelzesek += 2;
	printf("%-7s: Kezelo kesz!\n", "Elnok");
	}
}

void Kezelo()
{
	close(ellenorpipe[0]);
	close(ellenorpipe[1]);
	close(kezelopipe[0]);
	close(kezelopipe[1]);
	
	sleep(1);
	printf("%-7s: Itt vagyok!\n", "Kezelo");
	kill(getppid(), SIGUSR2);
}