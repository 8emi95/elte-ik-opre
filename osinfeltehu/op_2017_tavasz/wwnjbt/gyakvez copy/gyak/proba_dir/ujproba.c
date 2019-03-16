/*
A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására. A gyermekfolyamat indulása után csővezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra! A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevők. Ezt a partner képernyőre írja, majd várja a vendégeket és levezényli a rendezvényt! A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! (Minden résztvevő 10% valószínűséggel nem érkezik meg!) Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következő!
gekko42@gmail.com

*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>



char* helyszinek[] = {"Budapest","Keszthely", "London","Szombathely","Attyapuszta","Papa","Pozsony"};	// 7 rendezvény -> MAX gyermek szám
char* resztvevok[] = {"Katalin ", "Gabor ", "Barbara ", "Regina ", "Barnabas "};

volatile sig_atomic_t usr_interrupt = 0;


void handler(int signumber){
	usr_interrupt = 1;
	if(signumber == 10)
	{
		printf("Parent signal\n");
	}
	else if(signumber == 12)
	{
		printf("Child signal\n");
	}
	else
	{
		printf("Ctrl+c\n");
	}
}

void init_random(int i)
{
    srand(time(NULL) + i * 1024);
}

int generate_number(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

int random_participant()
{
	return generate_number(0,4);	// resztvevők max száma
}

void usage()
{
	printf("Hasznalat: bead2.o <rendezvenyek szama(1-7)>\n");
}


int main(int argc, char *argv[])
{
	
    sigset_t parent_sigset,	old_parent_sigset;
	sigset_t child_sigset,	old_child_sigset;
	
	sigemptyset(&parent_sigset); //empty signal set
	sigemptyset(&child_sigset); //empty signal set
	//sigaddset(&sigset,SIGINT); //SIGTERM is in set
	sigaddset(&parent_sigset,SIGUSR2); //SIGTERM is in set
	sigaddset(&parent_sigset,SIGUSR1); //SIGTERM is in set
	sigprocmask(SIG_BLOCK,&parent_sigset,NULL); //signals in sigset will be blocked
	//sigprocmask(SIG_BLOCK,&child_sigset,NULL); //signals in sigset will be blocked
	
	signal(SIGINT,handler);		// ha elakadna tudjam léptetni
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	
	pid_t child;
	pid_t parent_id = getpid();
	
	int pipefd[2];
	child = fork();
	if (child>0)
	{
		kill(child, SIGUSR2);
		if(!usr_interrupt)
		sigsuspend(&child_sigset);
		usr_interrupt=0;
		if(!usr_interrupt)
		kill(child, SIGUSR2);
		sigsuspend(&child_sigset);
	}
	else
	{
		sigsuspend(&child_sigset);
		kill(getppid(), SIGUSR1);
		sigsuspend(&child_sigset);
		kill(getppid(), SIGUSR1);
	}
/*
	
	
	if(pipe(pipefd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	int queue;
	key_t queue_key;
	
	queue_key = ftok(argv[0],1);
	queue = msgget(queue_key,0600 | IPC_CREAT);
	
	if(queue < 0)
	{
		perror("mssget error");
		return 1;
	}
	
		printf("Partner kikuldese helyszinre...\n");
		
		if((child = fork()) < 0)
		{
			perror ("fork error");
		
			abort();
		}
		///////////////////////
		/// szülő folyamat///
		//////////////////////
		else if(child > 0)	
		{
			
			
			
			
			int i;
			close(pipefd[0]);
			for(i = 0; i < MAXCHILD	; i++)
			{
				char nevek[100];
				nevek[0] = '\0';
				init_random(i);
				char *place = "";
				place = helyszinek[generate_number(0,MAXCHILD)];
				write(pipefd[1],place,strlen(place));
				
				
				kill(child,SIGUSR1);
				
				//sigprocmask (SIG_BLOCK, &parent_sigset, &old_parent_sigset);
				while (!usr_interrupt)
					sigsuspend (NULL);
				//sigprocmask (SIG_UNBLOCK, &parent_sigset, NULL);
				usr_interrupt = 0;
				
				
				int j;
				for(j = 0; j < random_participant(); j++)
				{
					strcat(nevek,resztvevok[j]); 
				}
				write(pipefd[1],nevek,strlen(nevek));
				memset(&nevek[0], 0, sizeof(nevek));
				
				
				kill(child,SIGUSR1);
				
				//sigprocmask (SIG_BLOCK, &parent_sigset, &old_parent_sigset);
				while (!usr_interrupt)
					sigsuspend (NULL);
				//sigprocmask (SIG_UNBLOCK, &parent_sigset, NULL);
				usr_interrupt = 0;
			}
			close(pipefd[1]);	
			
		}
		///////////////////////
		// gyerek folyamat//
		//////////////////////
		else if (child == 0)	
		{
			
			
			int i;
			for(i = 0; i < MAXCHILD; i++)
			{
				//sigprocmask (SIG_BLOCK, &child_sigset, &old_child_sigset);
				while (!usr_interrupt)
					sigsuspend (NULL);
				//sigprocmask (SIG_UNBLOCK, &child_sigset, NULL);
				usr_interrupt = 0;
				
				char helyszin[100];
				char nevek[100];
				close(pipefd[1]);
				int read_status=read(pipefd[0],helyszin,sizeof(helyszin));
				if(read_status == -1)
				{
					perror("pipe error");
				}
				printf("A partner megerkezett %s-i helyszinre(%d)\n",helyszin,i+1);
				memset(&helyszin[0], 0, sizeof(helyszin));		// tömb ürítés	
				
				
				kill(getppid(),SIGUSR2);			// megérkeztünk				
				
				//sigprocmask (SIG_BLOCK, &child_sigset, &old_child_sigset);
				while (!usr_interrupt)
					sigsuspend (NULL);
				//sigprocmask (SIG_UNBLOCK, &child_sigset, NULL);
				usr_interrupt = 0;
								
				read_status=read(pipefd[0],nevek,sizeof(nevek));
				if(read_status == -1)
				{
					perror("pipe error");
				}
				printf("%s\n",nevek);
				// TODO FELDOLGOZÁS
				memset(&nevek[0], 0, sizeof(nevek));
				
				kill(getppid(),SIGUSR2);
				
			}
			exit(0);			
		}*/
}
