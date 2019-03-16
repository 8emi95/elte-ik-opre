#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

#include <signal.h>
#include <sys/types.h>

volatile counter=0;
int pipefd[2]; 
int pipefd2[2];
struct mydata{
	int idvoter;
	int good;  //0- can vote
} ;

void handler(int signumber){
//  printf("Signal with number %i has arrived\n",signumber);
 printf("handler\n");
 counter++;
}
int numvoter;
void president()
{
  //pause();
  //pause();
  close(pipefd[0]);
  close(pipefd2[0]);
  close(pipefd2[1]);	
  printf("I am the president\n");
  while (counter!=2){usleep(100);}  
  printf("I got the signals from the children\n");
  printf("The number of the voters are %i\n",numvoter);
  int i,r;
  for (i=1;i<=numvoter;i++){
    r=rand();
    write(pipefd[1],&r ,sizeof(r));	
  }
  close(pipefd[1]);
}
void checker()
{
  close(pipefd[1]);	
  printf("I am the checker\n");	
//  sleep(1);
  kill(getppid(),SIGTERM);
  int idvoter,i;
 // for (i=1;i<=numvoter;i++){	 
  //read(pipefd[0],&idvoter,sizeof(idvoter));
  struct mydata d;
  while (read(pipefd[0],&idvoter,sizeof(idvoter))){
     printf("I got an identification number %i\n",idvoter);
	 d.idvoter=idvoter;
	 if ((rand()% 10) <2) {d.good=1;} else {d.good=0;}
	 write(pipefd2[1],&d,sizeof(d));
  }
  close(pipefd[0]);	
  close(pipefd2[1]);	
  
}
void sealer()
{
  close(pipefd[1]);	
  close(pipefd[0]);
  close(pipefd2[1]);	  
  printf("I am the sealer\n");	
//  sleep(1);
  kill(getppid(),SIGTERM);
  struct mydata d;
  while (read(pipefd2[0],&d,sizeof(d))){
     printf("I got an identification number %i %i\n",d.idvoter,d.good);
  }
  close(pipefd2[0]);
}
int main(int argc, char ** argv)
{
 int status;
 signal(SIGTERM,handler);
 numvoter=atoi(argv[1]); //checking existing or not
 srand(getpid());
     if (pipe(pipefd) == -1) 
	   {
               perror("Error in pipe\n");
               exit(EXIT_FAILURE);
           }
     if (pipe(pipefd2) == -1) 
	   {
               perror("Error in pipe\n");
               exit(EXIT_FAILURE);
           }		   
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
     pid_t child2=fork();
     if (child2<0){perror("The fork calling was not succesful\n"); exit(1);} 
     if (child2>0){
       president(); 		 
       waitpid(child2,&status,0);
       waitpid(child,&status,0); //wait(&status) 
     }else 
	 {
		 //second child
		 sealer();
	 }
 }
 else 
 {//1st child process
    checker();
 }
 return 0;
}