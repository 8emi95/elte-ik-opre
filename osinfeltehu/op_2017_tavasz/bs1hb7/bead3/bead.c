#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>
#include <unistd.h>  //fork
#include <signal.h>


void handler(int signumber){}





int main(int argc, char *argv[]){
	int fd;
	int pipefd;
	int pipefd1;
	int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
	int fid1=mkfifo("fifi.ftc", S_IRUSR|S_IWUSR );
	int status;
	signal(SIGTERM,handler);
	if (fid==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
if (fid1==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
			
				
			pid_t child1=fork();

				if(child1> 0) {
					
					printf("Taxi tarsasag\n");
					
					int v=0;
					char c[128];
					pipefd=open("fifo.ftc",O_RDONLY);
					pipefd1=open("fifi.ftc",O_WRONLY);

					while(1==1){	

						pause();
						read(pipefd ,c,sizeof(c));

						printf("Az utas uticelja: %s\n",c);

						pid_t child2 =fork();
						if(child2>0){
							waitpid(child1,&status,0);
						}else{
							//taxi
						}

					}	
					close(pipefd);
					close(pipefd1);
		
 				 }
  				else
  				{
     					
					pipefd=open("fifo.ftc",O_WRONLY);
					pipefd1=open("fifi.ftc",O_RDONLY);
	
					write(pipefd ,argv[1],sizeof(argv[1]));	
					kill(getppid(),SIGTERM);

					close(pipefd);
					close(pipefd1);
					
					exit(0);

			}
			

}
