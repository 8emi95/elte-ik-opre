#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h> 

int main()
{

	int f = open("fifo1", O_WRONLY);
	char uzenet[25] = "szia";
	write (f, uzenet, strlen(uzenet) + 1);
	close(f);
	
	return 0;
}
