#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>


void handler(int signalnumber){
 printf("The program was unpaused\n");
 
}

char name[80];

int main() {
	
	signal(SIGTERM,handler);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	 int pipeOzdBa[2]; // unnamed pipe file descriptor array
	 int pipeOzdBol[2];
	 int pipeKomloBa[2]; // unnamed pipe file descriptor array
	 int pipeKomloBol[2];
	
	   if (pipe(pipeOzdBa) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
		   
		   
	   if (pipe(pipeOzdBol) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }   
		   
	   if (pipe(pipeKomloBa) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
		   
		   
	   if (pipe(pipeKomloBol) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }   
		   
	
	
  int fork1 = fork();
  if (fork1 < 0) {
    printf("error\n");
  } else if (fork1 == 0) {
    printf("I'm child 1\n");
	 sleep(2);
  //  printf("child : parent: %i\n", getppid());
  //  printf("child 1: my pid: %i\n", getpid());
  //OZD
	 kill(getppid(),SIGTERM); 
	 
	 close(pipeOzdBa[1]);
	 char nyitIdo[80];  
	 read(pipeOzdBa[0],nyitIdo,80);
	 printf("kapott uzenet %s\n", nyitIdo);		
	 close(pipeOzdBa[0]);
	printf("Ozdi iroda");		 
	 
  } else {
    int fork2 = fork();
    if (fork2 < 0) {
      printf("error\n");
    } else if (fork2 == 0) {
      printf("I'm child 2\n");
	  sleep(2);
     // printf("child 2: parent: %i\n", getppid());
     // printf("child 2: my pid: %i\n", getpid());
	  kill(getppid(),SIGTERM); 
	  
	 close(pipeKomloBa[1]);
	 char nyitIdoK[80];  
	 read(pipeKomloBa[0],nyitIdoK,80);
	 printf("kapott uzenet %s\n", nyitIdoK);		
	 close(pipeKomloBa[0]);
	  printf("Komloi iroda");		
	  
    } else {
	  pause();
	  pause();
      printf("I'm the parent\n");
	  
	
	
		//printf("honap: %i ", &tm.tm_wday);
		
 time_t rawtime;
	char buffer [80];
/* 
	strftime (buffer, 80, "Today is %Y %A, %b %d.\n", tm);
	fputs (buffer, stdout);
	strftime (buffer, 80, "Time is %I:%M %p.\n", tm);
	fputs (buffer, stdout);
	
	
	tm->tm_mon++;
	mktime(tm);
	strftime (buffer, 80, "Month  is %b.\n", tm);
	fputs (buffer, stdout);
	
	
	 struct tm strtime;
    time_t timeoftheday;
 
    strtime.tm_year = 2008-1900;
    strtime.tm_mon = 1;
    strtime.tm_mday = 4;
    strtime.tm_hour = 02;
    strtime.tm_min = 30;
    strtime.tm_sec = 38;
    strtime.tm_isdst = 0;
	
	*/
	
	
	tm->tm_mday++;
	mktime(tm); /* Normalise ts */
	strftime (buffer, 80, " %Y:%b:%d", tm);
	//fputs (buffer, stdout);
	  
	  
	  write(pipeOzdBa[1], buffer, 30);
	  write(pipeKomloBa[1], buffer, 30);
	 
		 int status;
		wait(&status);
     // printf("The PIDs are:\n");
     // printf("parent: %i\n", getpid());
     // printf("child 1: %i\n", fork1);
     // printf("child 2: %i\n", fork2);
    }
  }
  return 0;
}