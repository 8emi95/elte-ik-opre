#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main( int argc, char** argv )
{
  pid_t child = fork();
  
  if( child < 0 )
  {
    perror( "CHILD process could not be generated!\n" );
  }
  else if( child == 0 )
  {
    printf( "I am the CHILD process!\nMy process id is: %i\n", (int) getpid() );
    exit( 42 );
  }  

  int status = 0;
  pid_t childId = wait( &status );    
  int childReturnValue = WEXITSTATUS( status );
  printf( "I am the PARENT process!\nMy CHILD's ID was: %d\nAnd Its return value was: %d\n", (int) childId, childReturnValue );
  
  return 0;
}
