#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main (int argc, char** argv)
{
	if (argc != 2)
	{
		perror("Not 1 argument");
		exit(2);
	}
	
	char cName[30];
	strcopy(cName, argv[1]);
	
	int f = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	
}