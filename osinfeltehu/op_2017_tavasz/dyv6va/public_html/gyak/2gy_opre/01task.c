#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber)
{
    printf("Signal with number %i has arrived\n", signumber);
}

int main(void)
{
	signal(SIGINT, handler);
	while(1)
	{
	}
}