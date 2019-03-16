#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error

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
    int nums[5];
    while(1) {
	    read(fd,s,sizeof(s));
	    printf("Ezt olvastam a csobol: %s \n",s);
        
    }
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
        }
	else // child
	{
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("fifo.ftc",O_WRONLY);
        int nums[5];
        int i;
        for (i = 0; i<=5; ++i) {
            nums[i] = rand()%100;
            printf("%d",nums[i]);
            char buf[2];
            sprintf(buf,"%d",nums[i]);
            write(fd, buf, 2);
            write(fd," ",1);
        }
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	}	
    
    return 0; 
}
