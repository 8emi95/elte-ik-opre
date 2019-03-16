#include <unistd.h>	//Fork
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit
//új
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>

int main(int argc, char* argv[])
{
	int n = 3;
	int fd;
	pid_t childs[n];
	pid_t parentPid = getpid();
    printf( "[Szülő] pid: %d\n",  parentPid);
	
	printf( "[Szülő] pid: %d - Fifo start!\n",  parentPid);
	
	int i;
    for ( i = 0; i < n; i++ )
	{
		pid_t pid = fork();
		if(getpid() == parentPid) childs[i] = pid;
        if ( pid == 0 )
        {
			pid_t childPid = getpid();
			pid_t myParentPid = getppid();
			
            printf( "[gyerek] pid: %d - szülő: %d\n", childPid, myParentPid );
			
			printf("[gyerek] pid: %d - irok a csobe!\n", childPid);
			
			char pipeName[15];
			sprintf(pipeName, "%d", childPid);
			
			// creating named pipe file
			int fid=mkfifo(pipeName, S_IRUSR|S_IWUSR );
			if (fid==-1)
			{
				printf("[gyerek] pid: %d - Error number: %i", childPid, errno);
				exit(EXIT_FAILURE);
			}
			
			fd=open(pipeName, O_WRONLY);
			write(fd,"Hajra Fradi!\n",12);
			close(fd);
			printf("[gyerek] pid: %d - beirtam a csobe!\n", childPid);
			
            exit( 0 );
        }
	}
    for ( i = 0; i < n; i++ )
	{
		char pipeName[15];
		sprintf(pipeName, "%d", childs[i]);
		
		sleep(3);
		char s[1024]="Semmi";		
		fd=open(pipeName,O_RDONLY);
		read(fd,s,sizeof(s));
		printf("[Szülő] pid: %d - Ezt olvastam a csobol: %s \n", parentPid, s);
		close(fd);
		// remove fifo.ftc
		unlink(pipeName);
		
		waitpid( childs[i] );
	}
	
	printf("[Szülő] pid: %d - leáll\n", parentPid);
}

