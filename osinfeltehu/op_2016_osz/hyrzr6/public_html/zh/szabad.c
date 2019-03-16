
// Operációs Rendszerek ZH
// Készítette: Csabai Tamás (HYRZR6)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h> 
#include <signal.h>

struct Jatekos{
	char nev[20];
	char feladat[50];
	int sorszam;
};

void handler(int signumber){
  printf("FELADAT VEGE JELZES!\n");
}

int main(int argc, char** argv ){
	
	if(argc != 2){
		printf("NEM MEGFELELO SZAMU ARGUMENTUM!\n");
		exit(EXIT_FAILURE);
	}
	
	int i;
	
	signal(SIGTERM,handler);
	
	int pid,fd;
	printf("START!\n");
	int fid=mkfifo("pipe.SZA", 0666 );
	if (fid == -1){
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
	}
	printf("FORK!\n\n");
	pid = fork();
	
	if(pid>0){   //tréner
		
		//char* argNev = ;
		struct Jatekos j1;
		strcpy(j1.nev, argv[1]);
		strcpy(j1.feladat, "FELADAT");
		j1.sorszam = 1;
		
		fd = open("pipe.SZA", O_WRONLY);
		printf("Trener: Irok a csobe!\n");
		write(fd,&j1,sizeof(j1));
		printf("Trener: Beirtam!\n");
		close(fd);
		fd = open("pipe.SZA", O_RDONLY);
		
		printf("Trener: Varom a jelzest...!\n");
		pause();
		printf("Trener: JELZES ERKEZETT!\n");
		
		int rrr;
		read(fd,&rrr,sizeof(rrr));
		close(fd);
		if(rrr >= 50) printf("Trener: SIKERES JATEKOS\n");
		else printf("Trener: SIKERTELEN JATEKOS\n");
		
		
		wait(NULL);
		close(fd);
		unlink("pipe.SZA");	
		
	
	}else{	//játékos
		
		struct Jatekos j1;
		fd = open("pipe.SZA", O_RDONLY);
		read(fd,&j1,sizeof(j1));
		printf("Jatekos: %d %s %s\n", j1.sorszam, j1.nev, j1.feladat);
		close(fd);
		
		int rrr = rand()%100;
		if(rrr >= 50) printf("Jatekos: SIKERES\n");
		else printf("Jatekos: SIKERTELEN\n");
		
		fd = open("pipe.SZA", O_WRONLY);
		printf("Jatekos: Irok a csobe!\n");
		write(fd,&rrr,sizeof(rrr));
		printf("Jatekos: Beirtam!\n");
		close(fd);
		
		printf("Jatekos: Kuldom a JELZEST!\n");
		sleep(1);
		kill(getppid(),SIGTERM);
			
	}

	return 0;
}