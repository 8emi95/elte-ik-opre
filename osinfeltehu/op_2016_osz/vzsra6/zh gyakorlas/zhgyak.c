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
		Ellenor();
	
	else
	{
		kezelo = fork();
		if (kezelo > 0)
			Kezelo();
		else
			Elnok();
	}
	
	
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
}

void Ellenor()
{
	printf("%-7s: Itt vagyok!\n", "Ellenor");
}

void KeszJel(int signumber)
{
	if (signumber == SIGUSR1 && jelzesek % 2 == 0)
	{
		jelzesek += 1;
	}
	else if (signumber == SIGUSR2 && jelzesek % 4 - jelzesek % 2 == 0)
	{
		jelzesek += 2;
	}
}

void Kezelo()
{
	printf("%-7s: Itt vagyok!\n", "Kezelo");
}