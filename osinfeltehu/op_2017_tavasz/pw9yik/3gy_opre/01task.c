
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error

struct Example
{
	int a;
	char b[80];
};

typedef struct Example Example;

int main(int argc, char* argv[])
{
    int pid, fd;

    printf("Fifo start!\n");
    int fid = mkfifo("fifo.ftc", S_IRUSR | S_IWUSR); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask
    if (fid == -1)
    {
        printf("Error number: %i", errno);
        exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
    pid = fork();

    if (pid > 0)   //parent
    {
        char s[1024] = "Semmi";
        printf("Csonyitas eredmenye szuloben: %d!\n", fid);
        fd = open("fifo.ftc", O_RDONLY);
		int i;
		for(i = 0; i < 5; i++)
		{
			int k;
			read(fd, k, sizeof(k));
			printf("Ezt olvastam a csobol: %d \n", k);
		}
		close(fd);
        // remove fifo.ftc
        unlink("fifo.ftc");
    }
    else     // child
    {
        printf("Gyerek vagyok, irok a csobe!\n");
        printf("Csonyitas eredmenye: %d!\n", fid);
        fd = open("fifo.ftc", O_WRONLY);
		int i;
		for(i = 0; i < 5; i++)
		{
			int random = rand() % 100;
			write(fd, random, sizeof(random));
			printf("Random szam: %i, memorai %i\n", random, &random);
		}
		close(fd);
        printf("Gyerek vagyok, beirtam, vegeztem!\n");
    }

    return 0;
}
