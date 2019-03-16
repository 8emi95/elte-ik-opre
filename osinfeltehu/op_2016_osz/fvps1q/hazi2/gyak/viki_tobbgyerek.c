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

int main()  
{
	
	
	const char * iroda1 = "Ózd";
	const char * iroda2 = "Komló";
	const char * datum = "2016:12:20";
	const char * datum2 = "2016:12:20";
	
    int cso_esemeny_hely[2];
	int cso_esemeny_hely2[2];
	
	char  csotartalom[1024];
	char  csotartalom2[1024];
	
	signal(SIGTERM,handler);
	

    pipe(cso_esemeny_hely) ;
	 pipe(cso_esemeny_hely2) ;

	pid_t gyerek1;
	pid_t gyerek2;
	gyerek1 = fork();
	
	
	if(gyerek1 > 0 )	
	{	
		
		gyerek2 = fork();
		
		if(gyerek2 >0)
		{
			pause();
			signal(SIGTERM,handler);
			pause();
			
			close(cso_esemeny_hely[0]);
			write(cso_esemeny_hely[1],datum,strlen(datum));
			close(cso_esemeny_hely[1]);
				printf("\nKartal(szülő):Az első csövet megírtam");
			close(cso_esemeny_hely2[0]);
				
			write(cso_esemeny_hely2[1],datum2,strlen(datum2));
			close(cso_esemeny_hely2[1]);
				printf("\nKartal(szülő):A második csövet megírtam");
			
				printf("\n\nKartal(szülő):A nyitási dátum elküldve %s\n", datum);
			
			//kill(gyerek1,SIGTERM);
			//kill(gyerek2,SIGTERM);
			
		
		wait(NULL);
		wait(NULL);

		}
		else{
			sleep(1);
			printf("Ózd: nyitásra kész:\n");
			kill(getppid(),SIGTERM);
			
			signal(SIGTERM,handler);
			sleep(4);
			
			
			close(cso_esemeny_hely[1]);
			read(cso_esemeny_hely[0],csotartalom,sizeof(csotartalom));
			close(cso_esemeny_hely[0]);
				printf("\nÓzd: A nyitási dátum fogadva.");
				printf("\nÓzd: Nyitás a következő napon: %s\n", csotartalom );
		}
		
		
	}else	
	{
		sleep(2);

		 printf("Karcag: nyitásra kész\n");
		 kill(getppid(),SIGTERM);
		 
		 signal(SIGTERM,handler);
		 sleep(5);
		 
		 
		 close(cso_esemeny_hely2[1]);
		 read(cso_esemeny_hely2[0],csotartalom2,sizeof(csotartalom2));
		 close(cso_esemeny_hely2[0]);
			printf("\nKarcag: A nyitási dátum fogadva.");
		 	printf("\nKarcag: Nyitás a következő napon: %s\n", csotartalom2 );
	
			
			
	}
	
	return 0;
}