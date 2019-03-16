#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

int main (void) 
{ 
  pid_t pid; 
  int mypipe[2]; 
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
    write(mypipe[1],"hello!\n",strlen("hello, world!\n")); 
    write(mypipe[1],"bye!\n",strlen("bye, world!\n")); 
    exit(EXIT_SUCCESS); 
  } 
} 
