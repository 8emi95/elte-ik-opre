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
 char c[1024];
};

int main(int argc, char* argv[])
{
    int pid,fd;
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
  
    pid = fork();
    
    if(pid>0)   //parent 
	{
	char s[1024]="Semmi";		
	printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	fd=open("fifo.ftc",O_RDONLY);
        struct a to_read;
	read(fd,&to_read,sizeof(struct a));
	printf("Ezt olvastam a csobol: %i %s \n",to_read.b, to_read.c);
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
        }
	else // child
	{
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
	struct a my_a;
    my_a.b = 10;
    strcpy (my_a.c ,"AAAA");
	strcat (my_a.c, " BBB");
	strcat (my_a.c, " VDSVDS");
	strcat (my_a.c, " CCCCCCC");
	strcat (my_a.c, " SVFCDSFG");
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,&my_a,sizeof(struct a));
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	}	
    
    return 0; 
}
