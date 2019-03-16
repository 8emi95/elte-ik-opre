

#include <stdio.h>
#include <signal.h>

void mysig(int s)
{
    printf("\nsignal: %d\n\n", s);
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, mysig);

    while ( 1 ) {
	printf(".");
	fflush(stdout);
	sleep(1);
    }

    return 0;
}