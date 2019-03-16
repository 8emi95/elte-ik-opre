#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

// Alarm handling
void CatchAlarm(int sig)
{
	printf("Alarm caught\n");
	alarm(5);
}

int main(void)
{
	signal (SIGALRM, CatchAlarm);
	int c;
	int text;
	
	alarm(5);
	
	while(1)
	{
		printf("\nAdd meg az allomas szamat (1-Ujpest 2-Obuda 0-Megse):\n");
		scanf("%d", &text);
		c = getchar();

		printf("A karakter %d\n", text);
		printf("A karakter %d\n", c);
	}
	return 0;
}
