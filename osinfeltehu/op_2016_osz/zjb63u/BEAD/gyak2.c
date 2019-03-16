#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#define MAX_SIZE 512
void arrived (int param)
{
	printf ("\npartner: A megadott helyszinen\n");
}

int main (void) 
{ 

  signal(SIGTERM,arrived);
  
  pid_t pid; 
  int mypipe[2]; 
  char readbuffer[MAX_SIZE];
  /*pipe létrehozása*/ 
  pipe (mypipe);
  /* child process letrehozasa */ 
  pid = fork ();
  if (pid > 0)
	{ 
    /*PARENT*/ 
	FILE *jelentkezettek;
	FILE *rendezveny;
	
	
	rendezveny = fopen("rendezveny.txt","r");
	
char line[MAX_SIZE];
char line2[MAX_SIZE];
while(fgets(line,sizeof(line),rendezveny) != NULL)
{
	char id[MAX_SIZE];
	char title[MAX_SIZE];
	jelentkezettek = fopen("jelentkezettek.txt","r");

	sscanf(line,"%s %s",id,title);
	
	write(mypipe[1], title ,strlen(title)+10); 
	pause();
	
	while(fgets(line2,sizeof(line2),jelentkezettek) != NULL)
	{
		char name[MAX_SIZE];
		char mail[MAX_SIZE];
		char j_id[MAX_SIZE];
		char asctime[MAX_SIZE];
		sscanf(line2,"%s %s %s %s",name,mail,j_id,asctime);
		
		if( strcmp(id,j_id) == 0 )
		{
			write(mypipe[1], name ,strlen(name)); 
			write(mypipe[1], "\n" ,strlen("\n")); 
		}
	}
	write(mypipe[1], "!" ,strlen("\n")); 
	sleep(6);
	read(mypipe[0],readbuffer,MAX_SIZE);
	printf ("\npartner -> foszervezo: %s\n", readbuffer);
	fclose(jelentkezettek);
}

	fclose(rendezveny);
	
    exit(EXIT_SUCCESS); 
  } 
	  
  if (pid == 0) 
  { 
    char c; 
	int profit;
	srand ( time(NULL) );
    /*CHILD*/ 
	while(read(mypipe[0],readbuffer,80))
	{
		printf ("\nfoszervezo -> partner: %s\n", readbuffer);
		sleep(3);
		kill(getppid(),SIGTERM); 
		sleep(4);
		printf ("\n\n--RENDEZVENYRE JELENTKEZETTEK--\n");
		read(mypipe[0],&c,1);
		int attendance = rand() % 100 + 1;
		int attend;
		char absentees[MAX_SIZE] = "";
		if (attendance > 10)
		{
			attend = 1;
		} else{
			attend = 0;
		}
		while (c != '!') 
		{ 
			putchar(c); 
			if (attend == 0)
			{
				absentees[strlen(absentees)] = c;
			}				
			if (c == 10)
			{
				attendance = rand() % 100 + 1;
				if (attendance > 10)
				{
					attend = 1;
				}else{
					attend = 0;
				} 
			}
			read(mypipe[0],&c,1);
		}
		printf ("\n--RENDEZVENY VEGE--\n");
		profit=random();
		char report[MAX_SIZE];
		snprintf(report, sizeof(report), "Jelentes:\nA rendezveny %i-Ft hasznot hozott\nA rendezvenyen nem megjelent szemelyek:\n",profit);
		write(mypipe[1],report,strlen(report)); 
		if(strlen(absentees) != 0)
		{
			write(mypipe[1],absentees,strlen(absentees)); 
		}else{
			write(mypipe[1],"-",100); 
		}
		sleep(4);
	}

    exit(EXIT_SUCCESS); 
   } 
} 
