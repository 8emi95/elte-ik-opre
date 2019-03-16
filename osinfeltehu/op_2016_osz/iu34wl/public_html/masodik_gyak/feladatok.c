#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h>

int main(int argc,char** argv)
{
	struct par
	{
		int szam;
		char szoveg[80];
	};
	
	struct par p;
	
	p.szam = 5;
	strcpy(p.szoveg, "Szovegecske\0");
	
	int file = open("./structkiir.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	write(file, &p, sizeof(p));
	close(file);
	
	struct par p2;
	int file2 = open("./structkiir.txt", O_RDONLY);
	read(file2, &p, sizeof(p));
	close(file);
	
	printf("%d %s \n", p.szam, p.szoveg);
	
	return 0;
}