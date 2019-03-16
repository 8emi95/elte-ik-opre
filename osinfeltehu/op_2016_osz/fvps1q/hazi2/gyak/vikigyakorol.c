#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main(int argc, char ** argv)  //karaktertömbökre mutató pointereket tartalmazó tömb
{
	
	
	const char * feladat = "elso";
	const char * sorszam = "2";
	const char * cso_fajlnev = "cso_txt";	
	
    int cso_esemeny_hely[2];
	char * cso_string[100]; 
	
	pid_t child; 
	
	signal(SIGTERM,handler);
	

    pipe(cso_esemeny_hely) ;
	
	
	
	int w;
	w = open(cso_fajlnev,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(w < 0)
	{
		perror("\nHiba a fájl megnyitásakor. A program kilép.\n");
		exit(1);
	}
	close(w);
	
	FILE * f;
	
	char t[1000];
	
	
	f = fopen(cso_fajlnev,"a+");
	fputs(feladat,f);
	fputs("\n",f);
	fputs(argv[1],f);
	fputs("\n",f);
	fputs(sorszam,f);
	fputs("\n",f);

	fclose(f);

	mkfifo(cso_fajlnev, S_IRUSR | S_IWUSR );
	
	
	child = fork();
	
	
	if(child > 0 )	
	{	
		
		printf("Megy a cucc\n");
	
      //  close(cso_esemeny_hely[1]);
		//kill(child,SIGTERM); 
		pause();
		
		close(cso_esemeny_hely[1]);
		read(cso_esemeny_hely[0],cso_string,sizeof(cso_string));
		close(cso_esemeny_hely[0]);
		printf("Szülő: A feladat megoldása: %s\n",cso_string);
		

		int status;
		wait(&status);
		
	}else	
	{
		FILE * p;
		char s[1024];
		
	//	pause();
		
		p = fopen(cso_fajlnev,"a+");
		printf("Gyerek megkapta az adatokat:\n");
		
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s\n",s);
		}        
		fclose(p);

		sleep(1);
		srand(time(NULL));
		int r = rand() % 2;
		char sikeres[10]="sikeres";
		char sikertelen[10]="sikertelen";
		close(cso_esemeny_hely[0]);
		if(r == 1)
		{ 
		printf("Gyerek: a feladat megoldása sikeres\n");
		write(cso_esemeny_hely[1],sikeres,sizeof(sikeres));
		}
		if (r==0)
		{ 
		printf("Gyerek: a feladat megoldása sikertelen\n");
		write(cso_esemeny_hely[1],sikertelen,sizeof(sikertelen));
		}
		close(cso_esemeny_hely[1]);

	
		kill(getppid(),SIGTERM);
		unlink(cso_fajlnev); 
	
	}
	
	return 0;
}