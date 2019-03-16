#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
#include "cstring.h"

struct Message {
	int  id;
	char text[80];
};

int main(int argc, char* argv[])
{
	struct Message msg;
	msg.id = 1;
	strcpy("Hello world!\n", msg.text);
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
	read(fd,s,sizeof(s));
	printf("Ezt olvastam a csobol: %s \n",s);
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
        }
	else // child
	{
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
    fd=open("fifo.ftc",O_WRONLY);
    char textmsg[30];
	sprintf(textmsg, "%i %s", msg.id, msg.text);
	write(fd, textmsg, 30);
    close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	}	
    
    return 0; 
}