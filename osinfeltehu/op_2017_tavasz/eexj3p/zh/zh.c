#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 

#define MAX_SIZE_CIM 100

void ittvagyok();
void megkaptam();

int main(int argc, char *argv[]) {
	if (argc == 2) {
		char cimTmp[MAX_SIZE_CIM];
		strcpy(cimTmp,argv[1]);
		printf("Param: %s\n", cimTmp);
		
		signal(SIGTERM,ittvagyok);
		signal(SIGUSR1,megkaptam);
		
		int status = 0;
		
		int p2c[2], n, m;                              /* pipe változó, olvasott bájtok száma */
		int c2p[2];
	
		pid_t pid;
		if (pipe(p2c) < 0)                          /* pipe létrehozása */	
			printf("Pipe hiba!\n");
			
		if (pipe(c2p) < 0)                          /* pipe létrehozása */	
			printf("Pipe hiba!\n");

		if ((pid = fork()) < 0)                     /* gyerekfolyamat létrehozása */
			printf("fork hiba");	
		else {
			if (pid == 0)                             
			{

				sleep(1);
				kill(getppid(),SIGTERM);
				printf("ez a child pid: %d \n", getpid());
				
				printf("Utas vagyok, kell egy kocsi!\n");
				
				
				close(p2c[0]);                          
				n = write(p2c[1], &cimTmp, MAX_SIZE_CIM);
				printf("A cimem ahova jonni kell: %s \n",cimTmp);
				sleep(2);
        
				fflush(stdout);
			}
			else {
				printf("Varakozas az utasra!\n");

				pause();
				printf("Van egy utas!\n");
				
				char kapottCim[MAX_SIZE_CIM];
				close(p2c[1]);                          
				m = read(p2c[0], &kapottCim, MAX_SIZE_CIM);         

				sleep(2);
				printf("Ide kuldom a taxit: %s\n", kapottCim);
				fflush(stdout);
				
				while ((wait(&status)) > 0);
				
			}
			return 0;
		}
    }
    else
    {
        printf("A program iditasakor adja meg parameterkent a lakcimet!!!!\n");
		return 0;
    }
	

}

void ittvagyok() {
	printf("Varom az utasokat!\n");
}

void megkaptam() {
	printf("Utas vagyok!\n");
}