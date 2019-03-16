#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signum) {
	//printf("Signal arrived! \n");
}

int main(int argc, char *argv[]) {
	int pipefd[2];
	pid_t pid;
	if(pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitásakor!");
		exit(1);
	}
	pid = fork();
	if(pid == -1) {
		perror("Hiba a forknál!");
		exit(1);
	}
	if(pid == 0) {
		//child proccess
		while(1) {
			signal(SIGUSR1, handler);
			pause();
			
			char rendezveny_neve[100];
			read(pipefd[0],rendezveny_neve,100);
			sleep(1);
			printf("PARTNER: Ittvagyok ezen a rendezvenyen: %s\n",rendezveny_neve);
			kill(getppid(),SIGUSR2);
			
			signal(SIGUSR1,handler);
			pause();
			int db;
			read(pipefd[0],&db,sizeof(int));
			int i;
			int nemjottekmeg = 0;
			char hianyzok[db][40];
			srand((unsigned) time(NULL));
			printf("PARTNER: A vart vendegek listaja:\n");
			for(i=0;i<db;i++) {
				char nev[40];
				read(pipefd[0],nev,40);
				printf("PARTNER: %i. %s\n",i+1,nev);
				if(rand() % 101 < 10) {
					int j = 0;
					for(j=0;j<40;j++) {
						hianyzok[nemjottekmeg][j] = nev[j];
					}
					nemjottekmeg++;
				}
			}
			int sikeresseg = rand() % 101;
			write(pipefd[1],&nemjottekmeg,sizeof(int));
			write(pipefd[1],&sikeresseg,sizeof(int));
			write(pipefd[1],hianyzok,40*nemjottekmeg);
			kill(getppid(),SIGUSR2);
		}
	}
	else {
		//parrent process
		char valasz = 'i';
		do {
			char rendezveny_neve[100];
			
			printf("FOSZERVEZO: irja be a rendezveny nevet (max 100 karakter): ");
			scanf("%s",rendezveny_neve);
			write(pipefd[1],rendezveny_neve,100);
			kill(pid,SIGUSR1);
			
			signal(SIGUSR2,handler);
			pause();
			printf("FOSZERVEZO: A partner megerkezett a rendezvenyre!\n");
			printf("FOSZERVEZO: Hany resztvevo lesz? ");
			int darabszam;
			scanf("%i",&darabszam);
			int i;
			write(pipefd[1],&darabszam,sizeof(int));
			for(i=0;i < darabszam;i++) {
				char nev[40];
				printf("FOSZERVEZO: %i. resztveto neve (max 40 karakter): ",i+1);
				scanf("%s", nev);
				write(pipefd[1],nev,40);
			}
			kill(pid,SIGUSR1);
			
			signal(SIGUSR2,handler);
			pause();
			int nemvoltakott, sikeresseg;
			read(pipefd[0],&nemvoltakott,sizeof(int));
			read(pipefd[0],&sikeresseg,sizeof(int));
			printf("FOSZERVEZO: %i\% volt a sikeresseg es ok nem erkeztek meg: \n",sikeresseg);
			for(i = 0; i < nemvoltakott; i++) {
				char nev[40];
				read(pipefd[0],nev,40);
				printf("FOSZERVEZO: %s\n",nev);
			}+
			printf("\nFOSZERVEZO: szeretne meg egy esemenyt lebonyolitani kedves felhasznalo? (i/n): ");
			scanf(" %c", &valasz);
		}while(valasz == 'i' || valasz == 'I');
		
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid,SIGKILL);
	}
	return 0;
}