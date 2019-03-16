#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>

int main(int argc, char* argv[])
{
    struct a
    {
      int b;
      char c[80];
    };

    int pid;
    int fd;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
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
    my_a.b = 20;
    strcpy (my_a.c,"Hajra HALI!!!");
    pid = fork();
    
    if(pid>0)   //parent 
	{
	struct a to_read;		
	printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	fd=open("fifo.ftc",O_RDONLY);
	read(fd,&to_read,sizeof(struct a));
	printf("Ezt olvastam a csobol: %d %s \n",to_read.b,to_read.c);
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
        }
	else // child
	{
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,&my_a,sizeof(struct a));
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	}	
    
    return 0; 
}
