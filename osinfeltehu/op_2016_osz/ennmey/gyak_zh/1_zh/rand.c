#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink
// -lrt -vel fordit!


int compare_strings(char a[], char b[])
{
   int c = 0;
 
   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
 
   if (a[c] == '\0' && b[c] == '\0')
      return 0;
   else
      return -1;
}

void beert(int signumber){
    if(1==signumber)
    {
        printf("Sikeresen beert a szerelveny.\n");
    }
    else
    {
        printf("Be lett vontatva.\n");
    }
}

int main (int argc, char* argv[]) { 

    srand(time(NULL));
    int i=0;
    for(i=0; i<15;++i)
    {
        printf("a szam: %d\n", rand()%3);
    }
     return 0; 
} 
