#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>



void handleralarm(int signalnumber){
 printf("\nThe program was alarmed by alarm function - stop function\n");
 raise(SIGTERM);
}

int main(int argc,char ** argv){
 signal(SIGALRM,handleralarm);
 if (argc!=2){
  perror("An argument is missing\n");
  exit(EXIT_FAILURE);
 }
 int sec=atoi(argv[1]);
 alarm(sec); //the system will send an ALARM signal after sec seconds
 printf("Starts\n"); 
 while (1){
  sleep(1); //running endless - stops the SIGTERM signal in handlealarm
  printf("."); fflush(stdout); //writes out immediatly
 } 
 return 0;
}