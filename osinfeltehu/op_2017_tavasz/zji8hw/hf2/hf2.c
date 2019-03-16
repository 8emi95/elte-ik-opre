#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

void handler_c(int sig)
{
	printf("Ezt a signalt kapta a child: %d\n", sig);
}

void handler_p(int sig)
{
	printf("Ezt a signalt kapta a parent: %d\n", sig);
}

void feladat1()
{
	pid_t child = fork();
	if (child < 0)
		perror("Creating child1");
	else if (child == 0)
	{
		signal(SIGUSR1, handler_c);
		kill(getppid(), SIGTERM);
		pause();
	}
	else
	{
		signal(SIGTERM, handler_p);
		pause();
		kill(child, SIGUSR1);
	}
}

void alarm_handler(int sig)
{
	time_t t = time(NULL);
	struct tm *ltime = localtime(&t);
	printf("\r%2.2d:%2.2d:%2.2d", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
}

void feladat2()
{
	setbuf(stdout, NULL);
	signal(SIGALRM, alarm_handler);
	do
	{
		alarm(1);
		pause();
	}
	while (1);
}

void handler_c1(int sig)
{
	printf("(P) Visszatert a child1\n");
}

void handler_c2(int sig)
{
	printf("(P) Visszatert a child2\n");
}

void feladat3()
{
	pid_t child1 = fork();
	if (child1 < 0)
		perror("Creating child1");
	else if (child1 == 0)
	{
		sleep(2);
		int rnd;
		do
		{
			rnd = rand() % 100 + 1;
			printf("(C2) %d\n", rnd);
		}
		while (rnd >= 10);
		kill(getppid(), SIGUSR1);
	}
	else
	{
		pid_t child2 = fork();
		if (child2 < 0)
			perror("Creating child2");
		else if (child2 == 0)
		{
			sleep(2);
			int rnd;
			do
			{
				rnd = rand() % 100 + 1;
				printf("(C1) %d\n", rnd);
			}
			while (rnd >= 10);
			kill(getppid(), SIGUSR2);
		}
		else
		{
			signal(SIGUSR1, handler_c1);
			signal(SIGUSR2, handler_c2);
			pause();
			pause();
		}
	}
}

void handler4_c1()
{
	printf("(P) child1 jelzest kuldott\n");
}

void handler4_c2()
{
	printf("(C2) parent jelzest kuldott\n");
}

void feladat4()
{
	pid_t child1 = fork();
	if (child1 < 0)
		perror("Creating child1");
	else if (child1 == 0)
	{
		sleep(1);
		kill(getppid(), SIGUSR1);
	}
	else
	{
		pid_t child2 = fork();
		if (child2 < 0)
			perror("Creating child2");
		else if (child2 == 0)
		{
			signal(SIGUSR2, handler4_c2);
			pause();
		}
		else
		{
			signal(SIGUSR1, handler4_c1);
			pause();
			kill(child2, SIGUSR2);
		}
	}
}

int main(int argc, char **argv)
{
	feladat4();
}