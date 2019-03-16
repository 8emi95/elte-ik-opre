
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/time.h> 

volatile int done_waiting = 0;

void handler( int signum ){
    if (signum == SIGUSR1)
    {
        done_waiting = 1;
    }    
}


int main(int argc, char** argv) {
	
	char temp[80];
	int fd, status;
    char versenyzoNeve[50];
    int feladatSorszama;
	int i=0;
	
    char feladatok[4][80];
	strcpy( feladatok[0], "Elso Feladat" );
	strcpy( feladatok[1], "Masodik Feladat" );
	strcpy( feladatok[2], "Harmadik Feladat" );
	strcpy( feladatok[3], "Negyedik Feladat" );
	
	char bukottak[argc][50];
	int bukottDB = 0;
	
	//argc==1 azaz nincs parameter, 	argv[0] maga a program neve
	//argc==2 azaz 1 paramater			argv[1] az elso parameter
    if (argc < 2) { 
        printf(" Legalabb egy parancssori parametert varok !"); 
        exit(1);
    }
    
	//cikluson kivulre mert amugy tul gyakran adja ua seedet!
	srand(time(NULL)); 
	
	int eddigeddig = argc;
	for(i=1; i<eddigeddig; i++){
		printf("\n i:%d 	edd:%d		i-argc:%d",i,eddigeddig,i-argc);
			
		int fid=mkfifo("adatcso", S_IRUSR|S_IWUSR );
		if (fid==-1)
		{
		printf("\nError number: %i",errno);
		exit(EXIT_FAILURE);
		}
			
		
		pid_t child = fork();
			
		//szulo folyamat:    
		if(child > 0){
					
			signal( SIGUSR1, handler );
			
			
			if(i<argc){
				strcpy( versenyzoNeve, argv[i] );
				
				feladatSorszama=(rand()%4) + 1; 
						
				fd=open("adatcso",O_WRONLY);			
					write(fd, versenyzoNeve, sizeof(versenyzoNeve) );
					printf("\n szulo: beirtam %s %d", versenyzoNeve, feladatSorszama );
					fflush(stdout);
					
					while( !done_waiting ) {
						sleep(1);
					}
					write(fd, &feladatok[ feladatSorszama - 1 ], sizeof(feladatok[ feladatSorszama - 1]));
					
					done_waiting = 0;
					
					sprintf( temp, "%d", feladatSorszama );
					while( !done_waiting ) {
						sleep(1);
					}
					write(fd, temp, sizeof(temp) );        
				close(fd);
				
						
				
				done_waiting = 0;
				while( !done_waiting ) {
						sleep(1);
					}
							
				fd=open("adatcso",O_RDONLY);		
				char fogadott_randomNr_str[10];        
				read(fd, fogadott_randomNr_str, sizeof(fogadott_randomNr_str));            
				close(fd);
				
				
				int fogadott_randomNr = atoi(fogadott_randomNr_str);
				if (fogadott_randomNr < 50) {
					printf("\n Szulo: Nem sikerult megoldani a feladatot %s resztvevonek! \n", versenyzoNeve );
					
					strcpy(bukottak[ bukottDB ], versenyzoNeve);
					printf("\n %s	%d", bukottak[bukottDB], bukottDB);
					bukottDB++;	
					eddigeddig++;
					
				}else {
					printf("\n Szulo: Sikerult megoldani a feladatot %s resztvevonek! \n", versenyzoNeve );
									
				}
				fflush(stdout);			
				close(fd);
				waitpid(child, &status,0);
			
			}else if(i>=argc){
				printf(" \n %d ", i - argc + 1);
				//printf(" \n %s ", bukottak[ i - argc + 1]);
			}
			
			
			
			unlink("adatcso");
			
			printf("\nA gyerek befejezodott!");
			printf("\nA szulo befejezodott!\n\n");       
		}
		
		
		
		//gyerek folyamat
		else if(child == 0) {	
			
			signal( SIGUSR1, handler );
			char fogadott_nev[50];
			char fogadott_feladat[80];
			char fogadott_sorszam_str[10];
			int fogadott_sorszam;
			
			fd=open("adatcso",O_RDONLY);
				read(fd, fogadott_nev, sizeof(fogadott_nev) );
				kill( getppid(), SIGUSR1);
				read(fd, fogadott_feladat, sizeof(fogadott_feladat) );
				kill( getppid(), SIGUSR1);
				read(fd, fogadott_sorszam_str, sizeof(fogadott_sorszam) );
				fogadott_sorszam = atoi( fogadott_sorszam_str );        
			close(fd);
			
			printf("\n %s : a %d sorszamu feladatot kaptam! ", fogadott_nev, fogadott_sorszam);
			
			int r=rand()%101; 
			if (r < 50) {
				printf("\n %s : Nem sikerult megoldani a %s feladatot! \n", fogadott_nev, fogadott_feladat);
			}else {
				printf("\n %s : Sikerult megoldani a %s feladatot! \n", fogadott_nev, fogadott_feladat);
			}
			fflush(stdout);
			
			
			kill( getppid(), SIGUSR1);        
	   
			fd=open("adatcso",O_WRONLY);
				char temp[10];
				sprintf(temp, "%d", r);
				write(fd, temp, sizeof(temp));            
			close(fd);
			
			kill( getppid(), SIGUSR1);
			exit(0);
		}
		else {
			printf(" Fork hiba! "); 
		}
	}
	
return 0;
}

