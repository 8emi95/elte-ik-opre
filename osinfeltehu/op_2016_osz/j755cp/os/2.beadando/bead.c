#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <math.h> 
#include <sys/ipc.h>
#include <sys/time.h>

//Globális változók
int gyerekjon=0;
int szulojon=1;
int resztvevoketkuld=0;
int resztvevoketfogad=0;
int status;
int gyerekvege=1;
int szulovege=1;


char buf[1000];
char filebuf[1000];
char data[1000];
char helyszin[100];

pid_t child;
int fd, fid;

char fajlnev[100];
char * input;
FILE * file;

void beolvas();
void rendezvenysikere();
void reszvetel (char input[]);
void sigint(int);
void siggyereknek(int s);
void sigszulonek(int s);
void tick(int tickTime);


int main(int argc, char* argv[])
{    	
	signal(SIGINT, sigint);
	signal(SIGUSR1, siggyereknek);
	signal(SIGUSR2, sigszulonek);
	
	unlink("fifo");
    int fid=mkfifo("fifo", S_IRUSR|S_IWUSR ); 
    if (fid==-1)
    {
		printf("fifo error\n");
		exit(EXIT_FAILURE);
    }

    child = fork();
    
    if(child==0)   //gyerekfolyamatban vagyunk 
	{
		while(gyerekvege==1)
		{
			fd=open("fifo",O_RDONLY);
			pause();
			if(gyerekjon==1)
			{
				if(resztvevoketfogad==0)
				{				
					read(fd,buf,sizeof(buf));
					printf("Megerkeztem a helyszinre:%s \n",buf);
					sleep(3);				
					resztvevoketfogad=1;
					kill(getppid(), SIGUSR2);
					fflush(stdout);
					pause();	
				}
				if (resztvevoketfogad==1)
				{
					read(fd,buf,sizeof(buf));
					printf("Varom a resztvevoket:\n%s\n",buf);
					sleep(3);
					printf("\nGyerek jelentese a rendezvenyrol:\n",buf);
					rendezvenysikere();
					reszvetel(buf);
					resztvevoketfogad=0;
					kill(getppid(), SIGUSR2);
					close(fd);
					fflush(stdout);
				}			
			}		
		}			
    }
	else if (child>0) //szülőfolyamatban vagyunk
	{
		while (szulovege==1)
		{
			fd=open("fifo",O_WRONLY);
			if(szulojon==1)
			{
				if(resztvevoketkuld==0)
				{
					printf("\n\n---- King of Stands rendezveny lebonyolitas ----\n");
					beolvas();					
					write(fd, helyszin, sizeof(helyszin));
					printf("Szulo beirta a helyszint!\n");
					sleep(3);
					resztvevoketkuld=1;
					kill(child, SIGUSR1);
					fflush(stdout);
					pause();								
				}
				if(resztvevoketkuld==1)
				{
					write(fd,data, sizeof(data));
					printf("Szulo beirta a resztvevoket!\n");
					sleep(3);	
					resztvevoketkuld=0;
					kill(child, SIGUSR1);
					close(fd);
					fflush(stdout);
					pause();
				}
			}
			//wait();
		}
    }
	unlink("fifo");
    return 0; 
}

void rendezvenysikere()
{
	srand(time(NULL));
	int r=rand()%100; 
	printf("\n***A rendezveny sikere: %d százalék!\n",r);
}

void reszvetel (char input[])
{
	printf ("\n***Az alabbi vendegek nem jottek el:\n");
	srand(time(NULL));
	char * ember;
    ember = strtok (input,"\n");
    while (ember != NULL)
    {
		int r=rand()%10;
		if(r<=1)
		{
			printf ("%s\n",ember);
		}
        ember = strtok (NULL, "\n");
    }
}

void beolvas()
{
	printf("Add meg a kovetkezo rendezveny adatait tartalmazo fajl nevet!\n");
	scanf("%s",&fajlnev);
	
	memcpy(helyszin, fajlnev, strlen(fajlnev) - 4);
	//memset(&filebuf[0], 0, sizeof(filebuf));
	memset(&data[0], 0, sizeof(data));
	file = fopen(fajlnev, "r");
	if (file == NULL)
	{
		printf("file opening error\n");
		exit(EXIT_FAILURE);
	}
	int i=0;
	while (!feof(file))
	{
		fgets(filebuf,sizeof(filebuf),file);
		strcat(data, filebuf);
	} 
	fclose(file);
}

void sigint(int s)
{
	gyerekvege=0;
	szulovege=0;
	signal(SIGINT ,sigint);
	unlink("fifo");
	printf("vége");
	exit(0);
}

void siggyereknek(int s)
{
	printf("-------MOST A GYEREK JON\n");
	fflush(stdout);
	gyerekjon=1;
	szulojon=0;	
	signal(SIGUSR1,siggyereknek); //reset signal
}

void sigszulonek(int s)
{
	printf("-------MOST A SZULO JON\n");
	fflush(stdout);
	szulojon=1;
	gyerekjon=0;
	signal(SIGUSR2,sigszulonek); //reset signal
}
