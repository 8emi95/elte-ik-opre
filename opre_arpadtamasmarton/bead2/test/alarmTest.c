#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
void CatchAlarm(int);

int main(void)
{
	signal (SIGALRM, CatchAlarm);
	alarm(5);
	
	int end;
	do
	{
		printf("0. Kilepes");
		printf("\n\n---------------\n");

		scanf("%d", &end);
		getchar();

	} while (end != 0);
	
	return 0;
}

void CatchAlarm(int sig)
{
	printf("Sikeres alarm\n\n");
	alarm(5);
}
