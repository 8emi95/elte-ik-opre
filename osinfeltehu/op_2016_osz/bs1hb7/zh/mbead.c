#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>




  
void handler(int signumber){
	//printf("wowshand %i\n",signumber);
	
	
}

struct uzenet{
  long mytype;
  char mtext [1024];
};
struct snet{
  long mytype;
  int ms;
  };



int main(int argc, char** argv){
	signal(SIGTERM, handler);
  
   //beolvasas
  
	srand(time(NULL));
	
    
        int usor=msgget(ftok(argv[0],1),0600|IPC_CREAT);
		//printf("pipe\n");
        if(usor<0){
          printf("Hiba a usor  nyitaskor!\n");
          exit(EXIT_FAILURE);
        }
        
        pid_t child2=fork();
		//printf("fork\n");
        if(child2==0)
        {
          //child2
          //printf("child\n");
          struct snet s;
          struct snet ossz;
          //printf("olvasa\n");
          msgrcv(usor,&s, sizeof(int),3,0);
         printf("%i db jatek van a voldon\n",s.ms);
          
          
          struct uzenet g={3,"csak varj, mig a kezunk koze kerulsz\n"};
          
          
	  msgsnd(usor,&g,129,0);
		  //printf("%i::%s\n",random,rnd);
		 
        kill(getppid(), SIGTERM);
        //printf("child2 exit\n");
		  exit(0);
		 
        }else{
			//szulo||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	//		printf("szulo\n");
	pid_t child1=fork();
	if(child1==0)
	{
	//child1
                
		struct snet o={3,(int)(rand()%30)+20};
		//printf("||||||||||%i,||||||",o.ms);
		
          msgsnd(usor, &o,  sizeof(o.ms),0);
        
          
		 
          
         // printf("child1 olvs\n");
          struct uzenet h;
          msgrcv(usor,&h,129,3,0);
		  
		printf("%s\n", h.mtext);
	  
          kill(getppid(), SIGTERM);   
        //printf("child1 exit\n");
        exit(0);
        
        }else
        {//szulo
        
        pause();
        pause();
        
        }
	}	  
      return 0;
  }


  
  
  