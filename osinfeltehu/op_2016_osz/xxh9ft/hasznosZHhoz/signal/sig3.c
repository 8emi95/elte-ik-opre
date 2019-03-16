#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
void Kezelo(int sig)
{
 printf("Ezt a jelzest kaptam%i\n",3);
 signal(SIGTERM,SIG_IGN);
 signal(SIGTERM,SIG_DFL);
 raise(sig);
 }
 int main(int argc, char * argv[])
 {
   signal(SIGTERM,Kezelo);
   return 0;
 }  