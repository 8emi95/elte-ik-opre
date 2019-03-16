#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main(int argc,char** argv)
{
	int db = atoi(argv[1]);
	int ellenor_jel = 0;
	int pecsetelo_jel = 0;
	signal(SIGTERM,handler);
	
	int w;
	w = open("pipe.file",O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(w);
	
	FILE * g;
	g = fopen("pipe.file","a+");
	int sz = 0;
	int i;
	for(i = 0;i<db;i++)
	{
		sz++;
		srand(time(NULL) + sz);
		int rnd = rand() % 1000000;
		char str[20];
		sprintf(str, "%i", rnd);
		strcat(str,"\n");
		fputs(str,g);
	}
	fclose(g);
	
	mkfifo("pipe.file", S_IRUSR | S_IWUSR );
	
	pid_t ellenor = fork();
	
	if(ellenor > 0)
	{
		pid_t pecsetelo = fork();
		
		if(pecsetelo > 0)
		{
			//elnök (szülő)
			printf("Elnök vagyok.\n");
			pause();
			signal(SIGTERM,handler);
			pause();
			printf("Elnök: Küldöm a szavazókat.\n");
			wait(NULL);
			wait(NULL);
			
		}else //pecsételő (2. gyerek)
		{
			sleep(1);
			printf("Pecsételő: Várom a szavazókat.\n");
			kill(getppid(),SIGTERM);
			//pause();
		}
	
	}else
	{
		//ellenőr (1. gyerek)
		sleep(2);
		printf("Ellenőr: Várom a szavazókat.\n");
		kill(getppid(),SIGTERM);
		sleep(1);
		printf("Ellenőr: Az alábbi szavazók érkeztek:\n");
		FILE * p;
		char s[1000];
		p = fopen("pipe.file","a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s", s);
		}        
		fclose(p);
		unlink("pipe.file");
		
		//pause();
	}
}



/*pid_t ellen = fork();

if (ellen > 0)
{
	pid_t pecsetelo = fork();
	if (pecsetelo > 0)
	{
		while(szamlalo != 2)
		{;} // pause helyett
		elnok();
		wait(NULL);
		wait(NULL);
	}else
	{
		pecset();
	}

}*/