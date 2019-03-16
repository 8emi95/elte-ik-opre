#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		perror("gyerek: Parameterkent add meg a cimet!\n");
		return 1;
	}

	printf("utas: jelentkezek\n");
	int addrfifo = open("y9sbt6", O_WRONLY);
	write(addrfifo, argv[1], sizeof(argv[1]));
	close(addrfifo);		
	
	return 0;
}