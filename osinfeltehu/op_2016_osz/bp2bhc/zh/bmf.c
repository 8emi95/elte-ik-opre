#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>

void success(int signum){
	printf("A metro beert a vegallomasra\n");
}

void failure(int signum){
	printf("A metrot be kellett vontatni\n");
}

int main(int argc, char **argv)
{
  signal(SIGUSR1, success);
  signal(SIGUSR2, failure);
  int parentpid = getpid();
  srand(time(NULL));
  int szerelvenyek = atoi(argv[1]);
  int uzenetsor, status;
  char* mqname="/metro_bp";
  struct mq_attr attr;
  mqd_t mq1,mq2;
  attr.mq_maxmsg=5;
  attr.mq_msgsize=50;
  char buffer[100];
  mq_unlink(mqname); // delete if exist
  mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
  if ( mq1 < 0 ) { 
     printf("mq_open error: %d \n",errno); 
     return 1; 
   } 

  int pid;
  pid = fork();
  if(pid > 0){
    /*szülõ*/
    int szerelvenyszam = (rand() % (szerelvenyek + 1)) + 1;
    char uzenet[30];
    sprintf(uzenet, "%d", szerelvenyszam);
    mq_send(mq1, uzenet, 30, 5);
    printf("uzenet elkuldve\n");
    wait( NULL ); 
    mq_receive(mq1, buffer, 100, NULL);
    printf("%s\n", buffer);
    mq_close(mq1);
    mq_unlink(mqname);
    printf("Szulo vege!\n");
    
  }else if(pid == 0){
    /*gyerek*/
    sleep(1);
    mq_receive(mq1, buffer, 100, NULL);
    printf("En vagyok a %s. metro\nUtanam meg %d van\n", buffer, szerelvenyek - atoi(buffer));
    int eredmeny = rand() % 10;
    if(eredmeny == 0){
        char *hiba = "Lefagyott a sin...";
        mq_send(mq1, hiba, 30, 5);
	kill(parentpid, SIGUSR2);
    }else{
        mq_send(mq1,"Minden jo", 30, 5);
        kill(parentpid, SIGUSR1);
    }
    exit(0);
  }


return 0;
}