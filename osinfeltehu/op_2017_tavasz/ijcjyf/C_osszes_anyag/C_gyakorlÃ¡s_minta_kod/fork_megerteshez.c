#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
   
   
pid_t child_a, child_b;

child_a = fork(); // a fork utánit masolja le 2x!!!!


//printf( "Azonostok:");

if (child_a == 0) {
   
    /* Child A code */
    printf( "Ez az en kodom:%d , ez a szulom kodja: %d \n", child_a , getppid() );
	  printf( "ide nem llép be?????????" );
    
} else {
     printf("\n ");
   //  printf( "Ez ezt ketszer le fogja irni :%d , ez a szulom kodja: %d \n ", child_b , getppid() );
    child_b = fork();
  //   printf( "Eezt talan csak egyszer fogja kiirni %d , ez a szulom kodja: %d \n ", child_b , getppid() );
    if (child_b == 0) {
        /* Child B code */
         printf( "En vagyok a masodik fork gyerek Ez az en kodom:%d , ez a szulom kodja: %d \n ", child_b , getppid() );
	
    } else {
        /* Parent Code */
         printf( "En vagyok a masodik fork  parent Ez az en kodom:%d , ez a szulom kodja: %d \n ", child_b , getppid() );
	
    }
}
     
}
