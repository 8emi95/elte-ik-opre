#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
void Kezelo(int sig)
{
 printf("Ezt a jelzest kaptam%i\n",sig);
  signal(SIGTERM,SIG_IGN);
   signal(SIGTERM,SIG_DFL);
    raise(sig);
    }
    int main(int argc, char * argv[]){
     printf("***************"); 
     signal(SIGTERM,Kezelo);
     printf("+++++++++++++++");
     return 0;
           }