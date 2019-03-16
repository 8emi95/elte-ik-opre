#include <stdio.h>
#include <string.h>

/* The index of the "read" end of the pipe */
#define READ 0

/* The index of the "write" end of the pipe */
#define WRITE 1

char *phrase = "Stuff this in your pipe and smoke it";

main () {
  int fd[2], bytesRead;

  char message [100]; /* Parent process message buffer */

  pipe ( fd ); /*Create an unnamed pipe*/
  
  if ( fork ( ) == 0 ) {
    /* Child Writer */
    close (fd[READ]); /* Close unused end*/
    write (fd[WRITE], phrase, strlen( phrase) +1); /* include NULL*/
    close (fd[WRITE]); /* Close used end*/
    printf("Child:  Wrote '%s' to pipe!\n", phrase);

  } else {

    /* Parent Reader */
    close (fd[WRITE]); /* Close unused end*/
    bytesRead = read ( fd[READ], message, 100);
    printf ( "Parent: Read %d bytes from pipe: %s\n", bytesRead, message);
    close ( fd[READ]); /* Close used end */
  } 
}