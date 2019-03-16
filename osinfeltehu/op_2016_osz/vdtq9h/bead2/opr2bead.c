/*
A "King of Stands" rendezvény szervezõ társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására. A gyermekfolyamat indulása után csõvezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra! A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevõket,kész a rendezvény lebonyolítására. Ezután a rendezvényszervezõ csõvezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevõk. Ezt a partner képernyõre írja, majd várja a vendégeket és levezényli a rendezvényt! A rendezvény végén jelentést küld vissza, amiben tájékoztatja a fõszervezõt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! (Minden résztvevõ 10% valószínûséggel nem érkezik meg!) Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következõ!
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


typedef struct app_persons
{
    long mtype;
    char names[256];
}Appeared_Persons;


typedef struct succes
{
    long mtype;
    char success[4];
}Success_Msg;



char* helyszinek[] = {"Budapest","Keszthely", "London","Szombathely","Attyapuszta","Papa","Pozsony"};	// 7 rendezvény -> MAX gyermek szám
char* resztvevok[] = {"Katalin ", "Gabor ", "Barbara ", "Regina ", "Barnabas "};

volatile sig_atomic_t usr_interrupt = 0;


void handler(int signumber){
	usr_interrupt = 1;
/*	if(signumber == 10)
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
	}*/
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
	return generate_number(1,5);	// resztvevõk max száma
}

void usage()
{
	printf("Hasznalat: bead2.o <rendezvenyek szama(1-7)>\n");
}

char* appeared(char* resztvevok)
{
	char ** res  = NULL;
	char *  p    = strtok (resztvevok, " ");
	int n_spaces = 0, i;



	while (p) {
		
	  res = realloc (res, sizeof (char*) * ++n_spaces);

	  if (res == NULL)
		exit (-1); 		

	  res[n_spaces-1] = p;

	  p = strtok (NULL, " ");
	}



	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = NULL;
	
	char* appeared_persons = malloc(sizeof(char)* (n_spaces+1)*10);		// 10 character mindenkire így biztos elég  a hely, ennél csak kevesebb lehet	
	
	appeared_persons[0] = '\0';
	int appeared_counter = 0;
	for(i = 0; (i < n_spaces); i++)
	{
		if(generate_number(1,10) > 1)	// 90% hogy megjelenik
		{
			strcat(appeared_persons, res[i]);
			strcat(appeared_persons, " ");
			appeared_counter++;
		}
	}
	
	return appeared_persons;
	
}



int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		usage();
		return -1;
	}
	const int MAXCHILD = atoi(argv[1]);
	if(MAXCHILD < 1 || MAXCHILD > 7)
	{
		usage();
		return -1;
	}
	
    sigset_t my_sigmask,	empty_sigmask;

	
	sigemptyset(&my_sigmask);
	sigemptyset(&empty_sigmask);
	
	sigaddset(&my_sigmask,SIGUSR1); //SIGTERM is in set
	sigaddset(&my_sigmask,SIGUSR2); //SIGTERM is in set
	
	sigprocmask(SIG_BLOCK,&my_sigmask,NULL); //signals in sigset will be blocked
	
	signal(SIGINT,handler);		// ha elakadna tudjam léptetni
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	
	pid_t child;
	pid_t parent_id = getpid();
	
	int pipefd[2];

	
	
	if(pipe(pipefd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	int queue;
	key_t queue_key;
	
	queue_key = ftok(argv[0],1);
	queue = msgget(queue_key,0600 | IPC_CREAT  );
	
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
		/// szülõ folyamat///
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
				
				
				
				
				sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				
				kill(child,SIGUSR1);
				while (!usr_interrupt)
                    sigsuspend (&empty_sigmask);
                usr_interrupt = 0;
				
				
				int j;
				for(j = 0; j < random_participant(); j++)
				{
					strcat(nevek,resztvevok[j]); 
				}
				write(pipefd[1],nevek,strlen(nevek));
				memset(&nevek[0], 0, sizeof(nevek));
				
				
				
				
				sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				
				kill(child,SIGUSR1);
				

				while (!usr_interrupt)
                    sigsuspend (&empty_sigmask);
                usr_interrupt = 0;
				
				
				Success_Msg Succ;
				Appeared_Persons appeared_persons;
				
				int msg_status = msgrcv(queue, &appeared_persons, 256, 1, MSG_NOERROR | IPC_NOWAIT);
				if(msg_status<0)
				{
					perror ("Msgrcv error");
				}
				
				
				msg_status = msgrcv(queue, &Succ, 4, 1, MSG_NOERROR | IPC_NOWAIT);
				if(msg_status<0)
				{
					perror ("Msgrcv error");
				}				
				printf("Megjelent emberek: %s\nEnnyire volt sikeres a rendezveny: %s\n",appeared_persons.names,Succ.success);
				memset(&appeared_persons.names[0], 0, sizeof(appeared_persons.names));
				memset(&Succ.success[0], 0, sizeof(Succ.success));
			}
			close(pipefd[1]);	
			msgctl(queue, IPC_RMID, NULL);
			printf("==================================\n");
		}
		///////////////////////
		// gyerek folyamat//
		//////////////////////
		else if (child == 0)	
		{
			
			printf("==================================\n");
			int i;
			for(i = 0; i < MAXCHILD; i++)
			{
				sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				
				while (!usr_interrupt)
                    sigsuspend (&empty_sigmask);
                usr_interrupt = 0;
				
				char helyszin[100];
				char nevek[1000];
				
				close(pipefd[1]);
				int read_status=read(pipefd[0],helyszin,sizeof(helyszin));
				if(read_status == -1)
				{
					perror("pipe error");
				}
				printf("A partner megerkezett %s-i helyszinre(%d)\n",helyszin,i+1);
				memset(&helyszin[0], 0, sizeof(helyszin));		// tömb ürítés	
							
				
				sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				kill(getppid(),SIGUSR2);
				while (!usr_interrupt)
                    sigsuspend (&empty_sigmask);
                usr_interrupt = 0;
								
				read_status=read(pipefd[0],nevek,sizeof(nevek));
				if(read_status == -1)
				{
					perror("pipe error");
				}
				printf("%s\n",nevek);
				// TODO FELDOLGOZÁS
				char* p =  appeared(nevek);
				
				
				Appeared_Persons persons;
				persons.mtype = 1;
				strcpy(persons.names,p);
				
				int msg_status;
                msg_status = msgsnd(queue, &persons, strlen(persons.names), 0);
                if (msg_status < 0)
                {
                    perror("msgsnd");
                }
								
				Success_Msg Succ;
				Succ.mtype = 1;
				sprintf(Succ.success, "%d\0", generate_number(1,100));
				
				
				msg_status = msgsnd(queue, &Succ, strlen(Succ.success), 0);
				if (msg_status < 0)
                {
                    perror("msgsnd");
                }

				free(p);
				memset(&nevek[0], 0, sizeof(nevek));
				kill(getppid(),SIGUSR2);
				
			}
			exit(0);			
		}
}



































