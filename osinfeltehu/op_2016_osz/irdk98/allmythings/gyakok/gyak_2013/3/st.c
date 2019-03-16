#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>



void handleralarm(int signalnumber){
 printf("The program was alarmed by alarm function\n");
}
int main(){
 //1. trial of creating a timer by alarm function
 signal(SIGALRM,handleralarm);
 alarm(1); //it is implemented in POSIX systems - but it is adviced to use setitimer
 sleep(2); //waiting the end of alarm BUT usually do not use sleep and alarm or setitimer together - may interference
 printf("\n\n");
 
 return 0;
}