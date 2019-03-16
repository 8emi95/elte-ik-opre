#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void arrived (int param)
{
	printf ("partner: A megadott helyszinen\n");
}

int main (void) 
{ 

  signal(SIGTERM,arrived);
  
  pid_t pid; 
  int mypipe[2]; 
  char readbuffer[80];
  /*pipe létrehozása*/ 
  pipe (mypipe);
  /* child process letrehozasa */ 
  pid = fork ();
  if (pid == 0) 
  { 
    char c; 
    /*CHILD*/ 
    //ezt a parent hasznalja, itt bezarhatjuk 
    close(mypipe[1]); 
	read(mypipe[0],readbuffer,80);
	kill(getppid(),SIGTERM); 
    //olvas, amig a szülõ be nem zárja a pipe másik végét 
    while (read(mypipe[0],&c,1)) 
    { 
      putchar(c); 
    } 
    exit(EXIT_SUCCESS); 
   } 
  else 
  { 
    /*PARENT*/ 
    //ezt a veget a child hasznalja 
    close(mypipe[0]); 
    //irunk a pipe-ra, a child majd kiolvassa 
    write(mypipe[1],"Szervezo: Rendezveny helye!\n",strlen("Szervezo: Rendezveny helye!\n")); 
	pause();
	write(mypipe[1],"hello!\n",strlen("hello, world!\n")); 
    write(mypipe[1],"bye!\n",strlen("bye, world!\n"));
    sleep(2); 
    exit(EXIT_SUCCESS); 
  } 
} 
