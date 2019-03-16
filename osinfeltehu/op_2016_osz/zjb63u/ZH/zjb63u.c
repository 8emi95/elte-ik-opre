#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <time.h>
#include <signal.h>

#define MAX_SIZE 32

int main (int argc, char** argv) 
{ 
  if (argc!=2)
  {
   perror("Nem kaptam meg az ossz szerelveny szamot!");
   exit(1);
   }
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
    while (read(mypipe[0],&c,1)) 
    { 
      putchar(c); 
    } 
    exit(EXIT_SUCCESS); 
   } 
  else 
  { 
    /*PARENT*/ 
	int osz_szerelveny;
	srand ( time(NULL) );
	osz_szerelveny = rand() % 100 + 1;
	int gy_szerelveny = *argv[1];
    //ezt a veget a child hasznalja 
    close(mypipe[0]); 
    //irunk a pipe-ra, a child majd kiolvassa 
	char uzenetcso[MAX_SIZE] = "";
	char szam[MAX_SIZE];
	//snprintf(uzenetcso, sizeof(uzenetcso), " %d\n",gy_szerelveny,osz_szerelveny);
	sprintf(szam,"%d", *argv[1]);
	uzenetcso[strlen(uzenetcso)] = *szam;
	uzenetcso[strlen(uzenetcso)] = ' ';
	sprintf(szam,"%d", osz_szerelveny);
	uzenetcso[strlen(uzenetcso)] = *szam;
	uzenetcso[strlen(uzenetcso)] = 10;
    write(mypipe[1],uzenetcso,strlen(uzenetcso)); 
    write(mypipe[1],"hello!\n",strlen("hello, world!\n")); 
    exit(EXIT_SUCCESS); 
  } 
} 
