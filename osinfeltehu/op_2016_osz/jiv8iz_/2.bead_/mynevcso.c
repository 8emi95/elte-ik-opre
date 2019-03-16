#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>
struct a
{
 int b;
 char c[80];
};

int main(int argc, char* argv[])
{
    int pid,fd;
    printf("Fifo start!\n");
    int fid=mknod("fifo", S_IFIFO | 0666, 0); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
    
	printf("Mkfifo vege, fork indul!\n");
    struct a my_a;
    my_a.b = 10;
    strcpy (my_a.c ,"AAAA");
    pid = fork();
    
    if(pid>0)   //parent 
	{
		char s[1024]="Semmi";	
		
		printf("Csonyitas eredmenye szuloben: %d!\n",fid);
		printf("Szulo elkezdi, elkuldi a helyszint. \n");
		fd=open("fifo",O_WRONLY);
		write(fd,"Budapest",9);
		close(fd);
		printf("Beirtam a helyszint, vegeztem!\n");
		
		
		// remove fifo.ftc
		unlink("fifo.ftc");
    }
	else // child
	{
		printf("Gyerek vagyok, irok a csobe!\n");
		printf("Csonyitas eredmenye: %d!\n",fid);
		fd=open("fifo",O_RDONLY);
       // struct a to_read;
	   char helyszin[100];
		read(fd,helyszin, sizeof(helyszin));
		printf("Ezt olvastam a csobol: %s \n",helyszin);
		close(fd);
    }	
    
    return 0; 
}
