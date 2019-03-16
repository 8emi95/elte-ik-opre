#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
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
	char event_id[100] = "VASÚTMODELLEK";	// rendezvény kódja/neve
	char event_place[100] = "SYMA Csarnok";	// rendezvény helyszíne
	
	const char * guest_filename = "guest.txt"; 			// eredeti vendéglistát tartalmazó fájl neve
	const char * pipe_filename = "pipe_guest_list.txt";	// csővezetékhez (vendéglista) használt fájl neve
	
	int pipe_event_place[2]; // csővezeték (helyszín)
    char pipe_string[100];  // karaktertömb csővezetékből való olvasáshoz
	
	pid_t child; // gyerekfolyamat ID
	
	// szignálkezelés
	signal(SIGTERM,handler);
	
	// csővezeték létrehozása
	if (pipe(pipe_event_place) == -1) 
	{
        perror("Hiba a csővezeték megnyitaskor!");
    }
	
	// csővezeték fájl létrehozása (vendéglista)
	int w;
	w = open(pipe_filename,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(w < 0)
	{
		perror("\nHiba a fájl megnyitásakor. A program kilép.\n");
		exit(1);
	}
	close(w);
	
	// csővezeték fájl feltöltése (vendéglista)
	FILE * f;
	FILE * g;
	char t[1000];
	
	f = fopen(guest_filename,"a+");
	g = fopen(pipe_filename,"a+");
	while(fgets(t,sizeof(char)*1000,f))
	{
		fputs(t,g);
	}
	fclose(f);
	fclose(g);
	
	// FIFO fájl létrehozása
	mkfifo(pipe_filename, S_IRUSR | S_IWUSR );
	
	system("clear");
	printf("\n---------------------------------------\nKING OF STANDS rendezvény lebonyolítása\n---------------------------------------\n\n");
	
	child = fork();
	
	if(child > 0 )	//központ
	{	
		// helyszín elküldése csővezetéken
		printf("1. esemény (központ): a központ megbízza a partnert a(z) %s rendezvény lebonyolítására.\n",event_id);
		close(pipe_event_place[0]);
		write(pipe_event_place[1],event_place,strlen(event_place));
        close(pipe_event_place[1]);
		pause();
		
		// vendéglista elküldése csővezetéken
		printf("6. esemény (központ): a központ elküldi a partnernek a résztvevők listáját.\n");
		sleep(1);
		kill(child,SIGTERM); // jelzés, hogy a gyerek olvashatja a FIFO fájlt
		
		int status;
		wait(&status);
		
	}else	//partner
	{
		FILE * p;
		char s[1024];
		
		// helyszín kiolvasása csővezetékből
		close(pipe_event_place[1]);
		sleep(1);
		read(pipe_event_place[0],pipe_string,sizeof(pipe_string));
		printf("2. esemény (partner): a partner megkapta a rendezvény helyszínét: %s\n",pipe_string);
		sleep(1);
		
		printf("3. esemény (partner): a partner a helyszínre utazik.\n");
		sleep(1);
		
		printf("4. esemény (partner): a partner a helyszínre ért.\n");
		sleep(1);
		
		// jelzés, hogy a gyerek várja a vendéglistát
		printf("5. esemény (partner): a partner a helyszínről jelez a központnak, hogy készen áll a látogatók fogadására.\n");
		sleep(1);
		kill(getppid(),SIGTERM);
		pause();
		
		// csővezeték fájl olvasása (vendéglista)
		printf("7. esemény (partner): a partner a következő vendéglistát kapta:\n");
		p = fopen(pipe_filename,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s", s);
		}        
		fclose(p);
		sleep(1);
		
		// rendezvény sikeressége
		srand(time(NULL));
		int r = rand() % 100;
		printf("8. esemény (partner): a rendezvény sikeressége: %i pont a 100-ból.\n",r);
		sleep(1);
		
		// el nem jött vendégek
		printf("9. esemény (partner): az alábbi vendégek nem jöttek el:\n");
		int a = 0;
		p = fopen(pipe_filename,"a+");
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
		
        unlink(pipe_filename); // csővezeték fájl törlése
	}
	
	return 0;
}