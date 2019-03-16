#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#include <sys/types.h>
#include <fcntl.h> // for O_ constants
#include <sys/ipc.h> //sem_open
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


//Global variables


int t2vPipe[2];
int v2tPipe[2];



struct vizsga_struct {
	char nev[50];
	char feladat[80];
	int sorszam;
	int eredmeny;
};



void handlerSIGUSR1(int sigNum)
{
	;
}



void vizsgazoProc()
{
	printf("Vizsgazo - Proc elindult\n");
	struct vizsga_struct vizsgaFel;

	close(t2vPipe[1]);
	close(v2tPipe[0]);


	read( t2vPipe[0] , &vizsgaFel, sizeof(vizsgaFel) );

	printf("Vizsgazo - neve: %s, feladat: %s, sorszam: %d\n", vizsgaFel.nev, vizsgaFel.feladat, vizsgaFel.sorszam);

	vizsgaFel.eredmeny = (random() % 5) + 1;

	
	printf("Vizsgazo - eredmeny : %d\n", vizsgaFel.eredmeny);

	write( v2tPipe[1] , &vizsgaFel, sizeof(vizsgaFel)  ) ;

	kill( getppid(), SIGUSR1 );
	
	printf("Vizsgazo - Proc Vege\n");

	close(t2vPipe[0]);
	close(v2tPipe[1]);
}


void trainerProc( char* argv[])
{
	printf("Trainer - Proc elindultam\n");


	close(t2vPipe[0]);
	close(v2tPipe[1]);

	struct vizsga_struct vizsgaFel;
	close( t2vPipe[0]);

	strcpy( vizsgaFel.nev, argv[1] );
	strcpy( vizsgaFel.feladat, "Elso feladat");
	vizsgaFel.sorszam = 1; 	

	write( t2vPipe[1] , &vizsgaFel, sizeof(vizsgaFel) );


	pause();  // Wait for signal



	read(v2tPipe[0], &vizsgaFel, sizeof(vizsgaFel)  );

	printf("Trainer - Vizsgazo eredmenye : %d\n", vizsgaFel.eredmeny);


	close(t2vPipe[1]);
	close(v2tPipe[0]);

	printf("Trainer - Proc vege\n");
}



int main (int argc, char* argv[])
{
	
	signal(SIGUSR1, handlerSIGUSR1 );

	if ( argc != 2)
	{
		printf("Kerem adjon meg egz nevet - egy parametert!\n ");
		exit(1);
	}

	pid_t vizsgazo;

    if ( pipe( t2vPipe ) < 0  )
    {   
        printf("Nem sikerult letrehozni a trainerToVizsgazo pipe-ot.\n");
        exit(2);
    }


    if ( pipe( v2tPipe ) < 0  )
    {   
        printf("Nem sikerult letrehozni a vizsgazoToTrainer pipe-ot.\n");
        exit(2);
    }


	srand( time(NULL));

	vizsgazo = fork();


	if ( vizsgazo < 0 )
	{
		perror("vizsgazo fork hiba");
		exit(1);
	}

	if ( vizsgazo == 0 )
	{
		//child
		vizsgazoProc();
		exit(0);
	}

	if ( vizsgazoProc > 0)
	{
		//parent
		trainerProc( argv );
		
		wait( NULL);
		exit(0);
	}
	



}

