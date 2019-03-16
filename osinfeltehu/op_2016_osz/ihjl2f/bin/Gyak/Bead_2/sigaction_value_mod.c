//// LINK with -lrt 
////send addional data with a signal

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
/*
union sigval {
   int   sival_int;  // the data is int, sent by a signal
   void *sival_ptr;  // or the data is a pointer!
};
*/                                         

struct student
{
  char name[80];
  int year;
};
  
  //struct student who={"Who am i?",2014}; // define a data structure to send
// signal handler 
void handler(int signumber,siginfo_t* info,void* nonused){
  printf("Signal with number %i has arrived\n",signumber);
  switch (info->si_code){ 
    case SI_QUEUE: printf("It was sent by a sigqueue, sending process (PID %i)\n",info->si_pid);
             //    printf("Additional value: %i\n",info->si_value.sival_int);
                   struct student* d=(struct student*)info->si_value.sival_ptr;                     
                   printf("Additional value: %s, %i\n",d->name,d->year);
                   break;
    default: printf("It was sent by something else \n");
  } 
}

int main(){
	struct student *bela = malloc(sizeof(struct student));
	strcpy(bela->name, "Bela");
	bela->year = 2016;

  struct student zoli={"Zoli",2014}; // define a data structure to send
  struct sigaction sigact;
  
  sigact.sa_sigaction=handler; //instead of sa_handler, we use the 3 parameter version
  sigemptyset(&sigact.sa_mask); 
  sigact.sa_flags=SA_SIGINFO; //we need to set the siginfo flag 
  sigaction(SIGTERM,&sigact,NULL); 

  printf("\n---------------------------------\n");  
  // timer_delete(t_id);	// timer delete
  pid_t child=fork();
  if (child>0)
  {
    pause();
    wait(NULL);
    printf("Parent process ended\n");
  }
  else 
  {
    //sendig an integer as an additional data 
    /*
    sleep(1);
    union sigval s_value_int={5};
    sigqueue(getppid(),SIGTERM,s_value_int); //just the same as kill function, but we can send additional data too 
    */
  struct student adam={"Barath Adam",2014}; // define a data structure to send
  struct student* ptr = &zoli;
  strcpy(ptr->name, "Fuck");
  ptr->year = 2111;
  
  strcpy(bela->name, "Bela mod");
	bela->year = 2030;
  
  printf("Pre send value: %s, %i\n",zoli.name,zoli.year);
    sleep(1);
    union sigval s_value_ptr;
    s_value_ptr.sival_ptr=bela;  //the struct data must define in commmon code
				//so &adam instead &zoli is a bad solution
    sigqueue(getppid(),SIGTERM,s_value_ptr);
    printf("Child process ended\n");  
  }
  return 0;
}
