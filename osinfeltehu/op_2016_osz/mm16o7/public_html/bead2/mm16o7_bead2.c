#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

#define MAX_SIZE 30

void clearBuff(char * str)
{
  memset(str,0,30);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int megjott()
{
  int t = rand()%10;
  if (t == 3)
  {
    return 0;
  }
  return 1;
}

int main (void) 
{ 
  pid_t pid; 
  int pwPipe[2];
  int cwPipe[2];
  char sz[MAX_SIZE];
  char gy[MAX_SIZE];
  
  char rendezvenyek[10][MAX_SIZE];
  char resztvevok[MAX_SIZE][MAX_SIZE];

  srand(time(NULL)); 
  
  char rendAzon[MAX_SIZE];
  char kiir[MAX_SIZE];
  int l;
  
  printf("\nAdja meg a rendezveny datumat (pl: 2016_12_31) :");
  scanf("%s",rendAzon);
  
  FILE *rendezveny;
  rendezveny = fopen(concat(rendAzon,".txt"),"r");
  //printf("OK 2\n");
  int resztvevo = 0;
  while(fgets(kiir,MAX_SIZE,rendezveny) != NULL)
  {
	//printf("OK 2esfel\n");
	for (l = 0; l < MAX_SIZE; ++l)
	{
		//printf("OK 2esharomnegyed\n");
		resztvevok[resztvevo][l] = kiir[l];
		//printf("OK 3\n");
	}
	++resztvevo;
  }
  //printf("resztvevoszam: %d\n", resztvevo);
	
  fclose(rendezveny);

  if (pipe(pwPipe) == -1) 
  {
	perror("Hiba a pipe nyitaskor!");
	exit(EXIT_FAILURE);
  }

  if (pipe(cwPipe) == -1)
  {
	perror("Hiba a masodik pipe nyitasakor!");
	exit(EXIT_FAILURE);
  }
   
  pid = fork (); 
  if (pid == -1) 
  {
    perror("Fork hiba");
    exit(EXIT_FAILURE);
  }
  
  if (pid == 0)		/*CHILD*/ 
  {  
	close(pwPipe[1]);
	close(cwPipe[0]);
    read(pwPipe[0],sz,sizeof(sz));
	printf("A helyszin: %s",sz);
	
	write(cwPipe[1], "A helyszinen vagyok!", strlen("A helyszinen vagyok!"));	
	printf("A resztvevok listaja:\n");
	clearBuff(sz);
	read(pwPipe[0],sz,sizeof(sz));
	int n = atoi(sz);
    //printf("resztvevoszam: %d\n", n);
	char megjottek[n][MAX_SIZE]; 

	int i = 0;
	int j;
	write(cwPipe[1],"OK",strlen("OK"));
	while(i < n)
	{
		clearBuff(sz);
		read(pwPipe[0],sz,sizeof(sz));
		printf("%s",sz);
		if (megjott() == 1)
		{
			for(j = 0; j < MAX_SIZE; ++j)
			{
			  megjottek[i][j] = sz[j];
			}
		}
		++i;
		write(cwPipe[1],"OK",strlen("OK"));
	}
	
	//printf("kijott a ciklusbol\n");
	read(pwPipe[0],sz,sizeof(sz));
	clearBuff(sz);
	int r = rand()%100;
	//printf("%d\n",r);
	sprintf(sz,"%d",r);
	write(cwPipe[1],sz,strlen(sz));

	read(pwPipe[0],sz,sizeof(sz));
	
	for (i = 0; i < n; ++i)
    {
		clearBuff(sz);
		for (j = 0; j < MAX_SIZE; ++j)
		{
			sz[j] = megjottek[i][j];
		}
		write(cwPipe[1],sz,sizeof(sz));
		read(pwPipe[0],sz,sizeof(sz));	
	}
	
	exit(EXIT_SUCCESS);
  } 
  else  			/*PARENT*/ 
  {  
    close(pwPipe[0]);
    close(cwPipe[1]);
    write(pwPipe[1],"Ulloi ut\n",strlen("Ulloi ut\n")); 
   
    read(cwPipe[0],gy,sizeof(gy));    
    
    clearBuff(gy);
    sprintf(gy,"%d",resztvevo);
    write(pwPipe[1],gy,strlen(gy));
    read(cwPipe[0],gy,sizeof(gy));
    int i;
    for (i = 0; i < resztvevo; ++i)
    {
		clearBuff(gy);
		for (l = 0; l < MAX_SIZE; ++l)
		{
			gy[l] = resztvevok[i][l];
		}
		write(pwPipe[1],gy,strlen(gy));
		read(cwPipe[0],gy,sizeof(gy));	
	}

    printf("Az esemeny sikeressege szazalekban:\n");
    clearBuff(gy);
    write(pwPipe[1],"OK",strlen("OK"));
    read(cwPipe[0],gy,sizeof(gy));
    printf("%s\n",gy);

	printf("Tenyleges resztvevok:\n");
    write(pwPipe[1],"OK",strlen("OK"));
    
    for (i = 0; i < resztvevo; ++i)
	{
		clearBuff(gy);
		read(cwPipe[0],gy,sizeof(gy));
		printf("%s",gy);
		
		write(pwPipe[1],"OK",strlen("OK"));
	}
    
    exit(EXIT_SUCCESS); 
  } 
} 
