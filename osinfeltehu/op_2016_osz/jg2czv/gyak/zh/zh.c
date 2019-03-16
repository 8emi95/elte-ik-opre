#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// --------------------------- //
// ---------- Fork ----------- //
// --------------------------- //

void handler(int signumber)
{
  // printf("\nSignal has arrived: %d\n", signumber);
}

int main(int argc, char* argv[])
{
  char name[100]="semmi!";
  strcpy(name, argv[1]);
  printf("%s",name);
  
  printf("Cso elinditasa!\n");
  int fid=mkfifo("pipe", S_IRUSR|S_IWUSR );
  if (fid==-1)
  {
    printf("Error number: %i",errno);
    exit(EXIT_FAILURE);
  }

  struct sigaction sigact;
  sigact.sa_handler=handler;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags=0;
  sigaction(SIGUSR1,&sigact,NULL);

  signal(SIGUSR1,handler);
  printf("A Trener keszen all a vizsga lebonyolitasara, gyerek inditasa!\n");
  pid_t child = fork();

  if(child>0)   //parent 
  {
      sigset_t sigset;
      sigfillset(&sigset);
      sigdelset(&sigset,SIGUSR1);
	  char message[500];
	  
      printf("\nTrener: Kuldom a feladatot, nevet es a sorszamot!");
      sprintf(message,"Feladat:%s \nNev:%s \nSorszam:%s", "feladat1",name,"2");
	  int fd=open("pipe",O_WRONLY);
		write(fd,message,sizeof(message));
		close(fd); 
	  
	  sleep(1);
	  char result[100]="Semmi!";
	  printf("\n","Trener: Varok az eredmenyre!");
      //pipe_read("pipe",result,sizeof(result));
	  fd=open("pipe",O_RDONLY);
	  read(fd,result,sizeof(result));
      close(fd);
	  unlink("pipe");
	  //sigsuspend(&sigset);
	  int status;
	  wait(&status);
	  printf("\nTrener: Megkaptam a jelzest, az eredmeny: %s\n", result);
	  
	   
  }
  else // child
  {
      char data[100]="Semmi"; 

      printf("\nVizsgazo: Varok a feladatra!\n");
		int fd=open("pipe",O_RDONLY);
	  read(fd,data,sizeof(data));
      close(fd);
      printf("\nVizsgazo: Megkaptam az uzenetet :\n %s\n", data);
	  
	  time_t t;
	  srand((unsigned) time(&t));
	  
	  char message[20] = "Nem sikerult";
	  int randomnumber = rand() % 100;
	  if (randomnumber > 50) strcpy(message,"Feladat megoldva!");
	  fd=open("pipe",O_WRONLY);
		write(fd,message,sizeof(message));
		close(fd); 
	  //pipe_put("pipe",message);
      
	  //kill(getppid(),SIGTERM);
	  //kill(getppid(),SIGTERM);
	  
	  

  }
  return 0;
}