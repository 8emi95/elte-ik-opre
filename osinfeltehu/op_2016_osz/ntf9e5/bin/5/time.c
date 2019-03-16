#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime

int main(){
 time_t t = time(0);
 printf("Time is: %s\n",ctime(&t)); //convert time_t to string 
 return 0;
}