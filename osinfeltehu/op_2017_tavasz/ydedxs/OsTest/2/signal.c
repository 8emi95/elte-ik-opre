#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){
	int pid,fd;
  srand(time(NULL));   // should only be called once
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  unlink("fifo1.ftc");
  int fid1=mkfifo("fifo1.ftc", S_IRUSR|S_IWUSR );
  if (fid1==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
    printf("Mkfifo ok\n");
  pid_t child=fork();
  if (child>0)
  { 
	sleep(1);
	int res ;
	fd=open("fifo1.ftc",O_RDONLY);
	pause(); //waits till a signal arrive 
    int r = rand()%100;  
	printf("parent :the number is %d\n",r);
    printf("Parent:Child is ready to play\n");
	printf("parent process:reading \n");
	read(fd,&res,10);
	printf("parent process:readed a %d \n",res);
    close(fd);
	int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    kill(getppid(),SIGTERM); 
	printf("child ready then send a SIGTERM %i signal\n",SIGTERM);
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
	int z = rand()%100; 
	printf("child process: writing  \n");
    fd=open("fifo1.ftc",O_WRONLY);
    write(fd,&z,10);
    printf("child process: writed  \n");
    close(fd);
	
    printf("Child process ended\n");  
  }
  return 0;
}