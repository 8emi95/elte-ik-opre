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
	char event_id[100] = "VASĂšTMODELLEK";	// rendezvĂ©ny kĂłdja/neve
	char event_place[100] = "SYMA Csarnok";	// rendezvĂ©ny helyszĂ­ne
	
	const char * guest_filename = "guest.txt"; 			// eredeti vendĂ©glistĂˇt tartalmazĂł fĂˇjl neve
	const char * pipe_filename = "pipe_guest_list.txt";	// csĹ‘vezetĂ©khez (vendĂ©glista) hasznĂˇlt fĂˇjl neve
	
	int pipe_event_place[2]; // csĹ‘vezetĂ©k (helyszĂ­n)
    char pipe_string[100];  // karaktertĂ¶mb csĹ‘vezetĂ©kbĹ‘l valĂł olvasĂˇshoz
	
	pid_t child; // gyerekfolyamat ID
	
	// szignĂˇlkezelĂ©s
	signal(SIGTERM,handler);
	
	// csĹ‘vezetĂ©k lĂ©trehozĂˇsa
	if (pipe(pipe_event_place) == -1) 
	{
        perror("Hiba a csĹ‘vezetĂ©k megnyitaskor!");
    }
	
	// csĹ‘vezetĂ©k fĂˇjl lĂ©trehozĂˇsa (vendĂ©glista)
	int w;
	w = open(pipe_filename,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(w < 0)
	{
		perror("\nHiba a fĂˇjl megnyitĂˇsakor. A program kilĂ©p.\n");
		exit(1);
	}
	close(w);
	
	// csĹ‘vezetĂ©k fĂˇjl feltĂ¶ltĂ©se (vendĂ©glista)
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
	
	// FIFO fĂˇjl lĂ©trehozĂˇsa
	mkfifo(pipe_filename, S_IRUSR | S_IWUSR );
	
	system("clear");
	printf("\n---------------------------------------\nKING OF STANDS rendezvĂ©ny lebonyolĂ­tĂˇsa\n---------------------------------------\n\n");
	
	child = fork();
	
	if(child > 0 )	//kĂ¶zpont
	{	
		// helyszĂ­n elkĂĽldĂ©se csĹ‘vezetĂ©ken
		printf("1. esemĂ©ny (kĂ¶zpont): a kĂ¶zpont megbĂ­zza a partnert a(z) %s rendezvĂ©ny lebonyolĂ­tĂˇsĂˇra.\n",event_id);
		close(pipe_event_place[0]);
		write(pipe_event_place[1],event_place,strlen(event_place));
        close(pipe_event_place[1]);
		pause();
		
		// vendĂ©glista elkĂĽldĂ©se csĹ‘vezetĂ©ken
		printf("6. esemĂ©ny (kĂ¶zpont): a kĂ¶zpont elkĂĽldi a partnernek a rĂ©sztvevĹ‘k listĂˇjĂˇt.\n");
		sleep(1);
		kill(child,SIGTERM); // jelzĂ©s, hogy a gyerek olvashatja a FIFO fĂˇjlt
		
		int status;
		wait(&status);
		
	}else	//partner
	{
		FILE * p;
		char s[1024];
		
		// helyszĂ­n kiolvasĂˇsa csĹ‘vezetĂ©kbĹ‘l
		close(pipe_event_place[1]);
		sleep(1);
		read(pipe_event_place[0],pipe_string,sizeof(pipe_string));
		printf("2. esemĂ©ny (partner): a partner megkapta a rendezvĂ©ny helyszĂ­nĂ©t: %s\n",pipe_string);
		sleep(1);
		
		printf("3. esemĂ©ny (partner): a partner a helyszĂ­nre utazik.\n");
		sleep(1);
		
		printf("4. esemĂ©ny (partner): a partner a helyszĂ­nre Ă©rt.\n");
		sleep(1);
		
		// jelzĂ©s, hogy a gyerek vĂˇrja a vendĂ©glistĂˇt
		printf("5. esemĂ©ny (partner): a partner a helyszĂ­nrĹ‘l jelez a kĂ¶zpontnak, hogy kĂ©szen Ăˇll a lĂˇtogatĂłk fogadĂˇsĂˇra.\n");
		sleep(1);
		kill(getppid(),SIGTERM);
		pause();
		
		// csĹ‘vezetĂ©k fĂˇjl olvasĂˇsa (vendĂ©glista)
		printf("7. esemĂ©ny (partner): a partner a kĂ¶vetkezĹ‘ vendĂ©glistĂˇt kapta:\n");
		p = fopen(pipe_filename,"a+");
		while(fgets(s,sizeof(char)*1024,p))
		{
			printf("%s", s);
		}        
		fclose(p);
		sleep(1);
		
		// rendezvĂ©ny sikeressĂ©ge
		srand(time(NULL));
		int r = rand() % 100;
		printf("8. esemĂ©ny (partner): a rendezvĂ©ny sikeressĂ©ge: %i pont a 100-bĂłl.\n",r);
		sleep(1);
		
		// el nem jĂ¶tt vendĂ©gek
		printf("9. esemĂ©ny (partner): az alĂˇbbi vendĂ©gek nem jĂ¶ttek el:\n");
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
		
        unlink(pipe_filename); // csĹ‘vezetĂ©k fĂˇjl tĂ¶rlĂ©se
	}
	
	return 0;
}