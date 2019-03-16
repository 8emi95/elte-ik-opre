#include <stdio.h>
#include <stdlib.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h> //rand
void handler(int sig)
{
	printf("Handler signal no. is: %i\n",sig);
}
int main(){
 
 //int f = mkfifo("fajlocska",0600);
 
 
 
 int f = open("fajlocska", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 printf("f = %i\n", f);
 
 char *c;// = "valamicske"; //"abcdefghi";
 c = (char *)malloc(10 * sizeof(char));
 strcpy(c, "valamicske");
 printf("c == %s\n", c);
 	
 write(f, c, 10 * sizeof(char));
 close(f);
 
 printf("access: %i\n", access("fajlocska", F_OK));
 
 
 int g = open("fajlocska", O_RDONLY);
 printf("g = %i\n", g);
 
 char *d;
 d = (char *)malloc(10 * sizeof(char));
 read(g, d, 10 * sizeof(char));
 close(g);
 printf("%s\n", d);
 
 /*pid_t child=fork();
 if (child>0)
  { //parent process
   printf("Parent waites for a while...\n");
   sleep(3);
   printf("Parent sends a SIGUSR1 signal to the child!\n");
   kill(child,SIGUSR1);
  // int i=rand()%100,status;
  // write(f,&i,sizeof(i)); //writes to the pipe
   printf("Parent waits for child end!\n");
   wait(NULL);   //waits for the child
   printf("Parent was finished too!\n");
  }
 else
  { // child process
   printf("Child ppoll is started!\n");
  signal(SIGUSR1,handler);
  struct timespec delay;
  delay.tv_sec=20;  // 20 seconds delay
  delay.tv_nsec=0;
  sigset_t sigmask;
  //signal(SIGINT, handler);
  sigfillset(&sigmask);  // sigmask is full now
  sigdelset(&sigmask,SIGUSR1); // SIGUSR1 removed from sigmask
				// it means ppoll accepts only SIGUSR1
  // poll - ppoll differences: timer (timespec), and sigmask(sigset_t)
  // if sigmask is NULL, no practical differences between poll-ppoll
  
  char *cc = "ablak";
  // int fd=open("fradi_cso",O_WRONLY);
  write(f, &cc, 6);
  // close(fd);
  printf("\n%i\n%i\n%i\n%i\n%i\n\n", POLLERR, POLLHUP, POLLNVAL, POLLIN, POLLOUT);
  
  int result=ppoll(poll_fds,1,&delay,&sigmask); //
  if (result>0) 
  {
   printf("\n%i\n\n", poll_fds[0].revents & POLLOUT);
   printf("The ppoll revents field is: %i\n",poll_fds[0].revents);
   if (poll_fds[0].revents & POLLIN) // POLLIN event occured
   {
	   printf("Now we can read from the pipe \n");
	   int data; char cdata;
	   read(f,&cdata,sizeof(cdata));   
	   printf("The data is: %c\n",cdata);                          
   }
  }
  else
  {
	printf("The returned ppoll result is: %i\n",result);
  	printf("Child process is over!\n");
  }
 }*/
 unlink("fradi_cso");
 return 0; 
}



int main2(int argc,char** argv){
 
 int f,g;
 
 g=open("kettesfajl", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 char c = 'B';
 write(g,&c,sizeof(c));
 close(g);
 
 
 f=open("kettesfajl",O_RDONLY);
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 
 char d;
 read(f,&d,sizeof(d));
 
 printf("%c\n", d);
 close(f);
 
 

 // while (read(f,&c,sizeof(c))){
    // printf("%c",c);
    // if (write(g,&c,sizeof(c))!=sizeof(c)) 
      // {perror("There is a mistake in writing\n");exit(1);}
 // } 
 
 return 0;
}