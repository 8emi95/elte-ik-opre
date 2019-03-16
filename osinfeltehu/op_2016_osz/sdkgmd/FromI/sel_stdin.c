#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h> //select
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close,select
#include <sys/types.h> //select
#include <sys/stat.h> //select
#include <sys/time.h> //rand
int main(){

 struct timeval tv;
 tv.tv_sec=5; //the select will wait for operation at last for 5 seconds
 tv.tv_usec=0; // the value of tv will change
 
 fd_set watchedfiles;
 FD_ZERO(&watchedfiles); //group of file descriptors
 FD_SET(0,&watchedfiles); //watch for standard input

 int result=select(1,&watchedfiles,NULL,NULL,&tv);
 //result = count of usable filedescriptors
 //result = 0 means, that timer worked
 //result < 0 means an error
 printf("result of select %i\n",result); 
 //select gives back control, when there is some change in any of file descriptors or there is a possibility to read or write
 //at the end of a file, we can read - so it gives back control : (
                           
 if (FD_ISSET(0,&watchedfiles)) //after select, only that filedesc are available 
                                //which are ready for operation
 { 
   printf("Now we read from the stdin \n");
   int data;
   read(0,&data,sizeof(data));   
   printf("The data is %c\nThe timer shows %i seconds back \n",data,tv.tv_sec);
                             
 } 
 else //it never happens, because we can read data or we are at the end of the file
 {
   printf("The timer was ticked\n"); 
 
 }
 
 return 0; 
}
