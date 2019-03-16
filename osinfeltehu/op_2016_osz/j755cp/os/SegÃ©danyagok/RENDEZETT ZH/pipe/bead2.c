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
#include <time.h>
#include <pwd.h> 

//globális flagek a signalhoz
int szamolnikell = 0;
int vege = 0;
int vanvalasz = 0;
int childvege = 0;
int childolvas = 1;
int konzolrololvas = 1;
	
//függvény defiíciók
void sigszamol(); 
char* removeNewLine();
int holvan(int, int, int, int, int, int);
void sigszamol(int);
void sigint(int);
void sigvalasz(int);

int main()
{

	signal(SIGINT, sigint);
	signal(SIGUSR1, sigszamol);
	signal(SIGUSR2, sigvalasz);

	unlink("fif0"); //ha már létezne
	mkfifo("fif0", 0666);
		
	
	pid_t child = fork();
	int status;
	if (child < 0) 
	{
        perror("fork");
        exit(1);
    }
	else if (child == 0) //gyerekfolyamatban van
	{	
		int fd1 = open("fif0", O_RDWR);
		int elso_x, masodik_x, elso_y, masodik_y, pont_x, pont_y; //pontok

        while (childvege == 0) //loop amíg vége
		{		
			if (childolvas == 1)
			{		
				read(fd1,&elso_x, sizeof(int)); //itt blokkol amíg nincs mit olvasni
				read(fd1,&elso_y, sizeof(int));
				read(fd1,&masodik_x, sizeof(int));
				read(fd1,&masodik_y, sizeof(int));
				read(fd1, &pont_x, sizeof(int));
				read(fd1,&pont_y, sizeof(int));
			
				szamolnikell = 1;
				childolvas = 0;
			}
			if (szamolnikell == 1)
			{
				printf("%d%d%d%d%d%d\n", elso_x, elso_y, masodik_x, masodik_y, pont_x, pont_y);
				int hol = holvan(elso_x, elso_y, masodik_x, masodik_y, pont_x, pont_y);
				fflush(stdout);
				
				
				kill(getppid(), SIGUSR2);	
				write(fd1, &hol, sizeof(int));

				szamolnikell = 0;				
			}
		}
	}
	else if (child > 0)//szülõfolyamatban van õ kérdezi, hogy hol van a pont az egyeneshez képest
	{
		int fd2;
		while (vege == 0)
		{
			if (konzolrololvas == 1)
			{
				int elso_x, masodik_x, elso_y, masodik_y, pont_x, pont_y = 1;
				printf("elso_x: "); elso_x = beolvas();
				printf("elso_y: "); elso_y = beolvas();
				printf("masodik_x: "); masodik_x =beolvas();
				printf("masodik_y: "); masodik_y = beolvas();
				printf("pont_x: "); pont_x = beolvas();
				printf("pont_y: "); pont_y = beolvas();
				
				
				fd2 = open("fif0", O_WRONLY);
				//az egyenes koordinátái
				write(fd2, &elso_x, sizeof(int)); 
				write(fd2, &elso_y, sizeof(int));
				write(fd2, &masodik_x, sizeof(int));
				write(fd2, &masodik_y, sizeof(int));
				
				//a kérdéses pont koordinátái
				write(fd2, &pont_x, sizeof(int));
				write(fd2, &pont_y, sizeof(int));
				
				
				kill(child, SIGUSR1); 
				konzolrololvas = 0;
				close(fd2);
				
			}
			
			if (vanvalasz == 1)
			{
				fd2 = open("fif0", O_RDONLY);
				int hol;
			
				read(fd2, &hol, sizeof(int));
				if (hol > 0)
					printf("egyenes felett.\n");
				else if (hol < 0)
					printf("egyenes alatt.\n");
				else 
					printf("az egyenesen.\n");
					
				fflush(stdout);
				close(fd2);

				konzolrololvas = 1;
				vanvalasz = 0;
			}
		}
	}
	
	return 0;
}


char* removeNewLine(char* str)
{
	if (str != NULL)
    {
		char *newline = strchr(str, '\n'); //visszaadja h hol van az str-ben a '\n', pointerként
		if (newline != NULL)
		{
			*newline = '\0'; 
		}
    }
}

int beolvas()
{
	char* tmp = (char*)malloc(50*sizeof(char));
	fgets(tmp, 50, stdin);
	removeNewLine(tmp);
	return atoi(tmp);
}

int holvan(int elso_x, int elso_y, int masodik_x, int masodik_y, int pont_x, int pont_y)
{
	int ertek = (pont_y - elso_y) * (masodik_x - elso_x) - (pont_x - elso_x) * (masodik_y - elso_y);
	return ertek;
}

void sigszamol(int s)
{
	//szamolnikell = 1; //így csinál valamit a gyerek folyamat
	printf("SIGSZAMOL\n");
	childolvas = 1;
	signal(SIGUSR1,sigszamol); //reset signal
}

void sigint(int s)
{
	signal(SIGINT ,sigint);
	printf("vége");
	unlink("fif0");
	vege = 1;
	childvege = 1;
	exit(0);
}

void sigvalasz(int s)
{
	printf("SIGVALASZ\n");
	vanvalasz = 1;
	signal(SIGUSR2,sigvalasz); //reset signal
}
