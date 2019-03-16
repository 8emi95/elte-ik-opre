#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main()
{
	char esemeny_azon[100] = "CICAKIÁLLÍTÁS";	
	char esemeny_hely[100] = "Várkert Bazár";	
	
	
	const char * cso_fajlnev = "vendeglista_csohoz.txt";	
	const char * vendeg_fajlnev = "vendeg.txt"; 			
	
	
	int cso_esemeny_hely[2]; 
    char cso_string[100]; 
	
	pid_t child; 
	

	signal(SIGTERM,handler);
	

	if (pipe(cso_esemeny_hely) == -1) 
	{
        perror("Hiba a csővezeték megnyitaskor!");
    }
	
	
	int w;
	w = open(cso_fajlnev,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(w < 0)
	{
		perror("\nHiba a fájl megnyitásakor. A program kilép.\n");
		exit(1);
	}
	close(w);
	
	
	FILE * f;
	FILE * g;
	char t[1000];
	
	f = fopen(vendeg_fajlnev,"a+");
	g = fopen(cso_fajlnev,"a+");
	while(fgets(t,sizeof(char)*1000,f))
	{
		fputs(t,g);
	}
	fclose(f);
	fclose(g);
	
	
	mkfifo(cso_fajlnev, S_IRUSR | S_IWUSR );
	
	system("clear");
	printf("\nKING OF STANDS rendezvény lebonyolítása\n\n");
	
	child = fork();
	
	if(child > 0 )	
	{	
		
		printf("Központ: megbízza a partnert a(z) %s rendezvény lebonyolítására.\n",esemeny_azon);
		close(cso_esemeny_hely[0]);
		write(cso_esemeny_hely[1],esemeny_hely,strlen(esemeny_hely));
        close(cso_esemeny_hely[1]);
		pause();
		
		
		printf("Központ: elküldi a partnernek a résztvevők listáját.\n");
		sleep(1);
		kill(child,SIGTERM); 
		
		int status;
		wait(&status);
		
	}else	
	{
		FILE * p;
		char s[1024];
		
		
		close(cso_esemeny_hely[1]);
		sleep(1);
		read(cso_esemeny_hely[0],cso_string,sizeof(cso_string));
		printf("Partner: megkapta a rendezvény helyszínét: %s\n",cso_string);
		sleep(1);
		
		printf("Partner: elindult a helyszínre.\n");
		sleep(1);
		
		printf("Partner: a helyszínen van.\n");
		sleep(1);
		
		
		printf("Partner: a helyszínről jelez a központnak, hogy készen áll a látogatók fogadására.\n");
		sleep(1);
		kill(getppid(),SIGTERM);
		pause();
		
		
		printf("Partner: a következő vendéglistát kapta:\n");
		p = fopen(cso_fajlnev,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s", s);
		}        
		fclose(p);
		sleep(1);
		
		
		srand(time(NULL));
		int r = rand() % 50;
		printf("Partner: a rendezvény sikeressége: %i pont a 50-ból.\n",r);
		sleep(1);
		
		
		printf("Partner: a következő vendégek nem jelentek meg:\n");
		int a = 0;
		p = fopen(cso_fajlnev,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			a++;
			srand(time(NULL) + a);
			int r2 = rand() % 10;
			if(r2 == 1)
			{
				printf("%s", s);
			}
		}
		fclose(p);
		
        unlink(cso_fajlnev); 
	}
	
	return 0;
}