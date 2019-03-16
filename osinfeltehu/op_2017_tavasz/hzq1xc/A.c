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
    printf("Pamacs start!\n");
    int fid=mkfifo("pamacs", S_IRUSR|S_IWUSR ); // creating named pipe file
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
	fd=open("pamacs",O_RDONLY);
	read(fd,s,sizeof(s));
	printf("Ezt olvastam a csobol: %s \n",s);
	close(fd);
	// remove fifo.ftc
	wait( NULL ); 
	unlink("pamacs");
        }
	else // child
	{
	int i;
	 //printf("Number of command line arguments are: %i\n",argc);
	 char s[1024]="";	
	 for (i=1;i<argc;i++){
	  //printf("%i. argument is %s\n",i,argv[i]);
	  sprintf(s, "%s %s",s, argv[i]);
	 }
	 //printf("%s",s);
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("pamacs",O_WRONLY);
        write(fd,s,1024);
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    	}	
    
    return 0; 
}