#include <signal.h> //signalok
#include <stdio.h>	//Fájl kezelés + kiiratás

void	Elnok();
void	Ellenor();
void	KeszJel();
void	Kezelo();

int jelzesek = 0;

int main(int argc, char** argv)
{
	int ellenor, kezelo;
	
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
	
	
	
	int status;
	wait(&status);
	wait(&status);
	printf("%-7s: Mindenki halott! :(\n", "Elnok");
}

void Ellenor()
{
	printf("%-7s: Itt vagyok!\n", "Ellenor");
	kill(getppid(), SIGUSR1);
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
	printf("%-7s: Itt vagyok!\n", "Kezelo");
	kill(getppid(), SIGUSR2);
}