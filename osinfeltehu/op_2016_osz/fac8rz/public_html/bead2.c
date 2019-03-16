
// FAC8RZ
// gyurjan.marcell@gmail.com


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


typedef struct guests
{
    long mtype;
    char names[256];
} guests_appeared;


typedef struct succes
{
    long mtype;
    char success[4];
} Success_Msg;


char* locations[] = { "Tokio", "New York", "Sanghai", "Delhi", "Paris", "Los Angeles", "Toronto" };
char* visitors[] = { "Dave ", "Miranda ", "Sylvia ", "Omar ", "John "}; 

volatile sig_atomic_t usr_interrupt = 0;


void handler(int signumber)
{
	usr_interrupt = 1;
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
	return generate_number(1,5);	
}

void usage()
{
	printf("Parameterezes: bead2.o < rendezvenyszam (1-7) >\n");
}

char* appeared(char* visitors)
{
	char** res = NULL;
	char* p = strtok (visitors, " ");
	int n_spaces = 0, i;


	while(p) 
	{		
	  res = realloc (res, sizeof (char*)* ++n_spaces);

	  if (res == NULL)
	  {
		exit(-1); 		
	  }
	  
	  res[n_spaces-1] = p;
	  p = strtok (NULL, " ");
	}


	res = realloc (res, sizeof (char*)* (n_spaces+1));
	res[n_spaces] = NULL;
	
	char* guests_appeared = malloc(sizeof(char)* (n_spaces+1)*10);		
	
	guests_appeared[0] = '\0';
	int appeared_counter = 0;
	for(i = 0; (i < n_spaces); i++)
	{
		if(generate_number(1,10) > 1)	
		{
			strcat(guests_appeared, res[i]);
			strcat(guests_appeared, " ");
			appeared_counter++;
		}
	}
	
	return guests_appeared;
	
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
	
    sigset_t my_sigmask, empty_sigmask;

	sigemptyset(&my_sigmask);
	sigemptyset(&empty_sigmask);
	
	sigaddset(&my_sigmask, SIGUSR1);
	sigaddset(&my_sigmask, SIGUSR2);
	
	sigprocmask(SIG_BLOCK, &my_sigmask, NULL); 
	
	signal(SIGINT, handler);	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	pid_t child;
	pid_t parent_id = getpid();
	
	int pipefd[2];
	
	if(pipe(pipefd) == -1)
	{
		perror("pipe hiba");
		exit(EXIT_FAILURE);
	}
	
	int queue;	
	key_t queue_key;
	
	queue_key = ftok(argv[0], 1);
	queue = msgget(queue_key, 0600 | IPC_CREAT  );
	
	if(queue < 0)
	{
		perror("mssget hiba");
		return 1;
	}
	
	printf("\n");	
	printf("Sending partner to target location ...\n");
		
	if((child = fork()) < 0)
	{
		perror ("fork hiba");
		abort();
	}
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
			place = locations[generate_number(0, MAXCHILD)];
			write(pipefd[1], place, strlen(place));
				
			sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				
			kill(child, SIGUSR1);
			while(!usr_interrupt)
			{
				sigsuspend (&empty_sigmask);
			}
			usr_interrupt = 0;
				
			int j;
			for(j = 0; j < random_participant(); j++)
			{
				strcat(nevek, visitors[j]); 
			}
			write(pipefd[1],nevek,strlen(nevek));
			memset(&nevek[0], 0, sizeof(nevek));
				
			sigprocmask (SIG_BLOCK, &my_sigmask, NULL);		
			kill(child,SIGUSR1);
				
			while (!usr_interrupt)
			{
                sigsuspend (&empty_sigmask);
			}			
            usr_interrupt = 0;
				
			Success_Msg Succ;
			guests_appeared guests_appeared;
				
			int msg_status = msgrcv(queue, &guests_appeared, 256, 1, MSG_NOERROR | IPC_NOWAIT);
			if(msg_status < 0)
			{
				perror ("Msgrcv hiba");
			}
				
			msg_status = msgrcv(queue, &Succ, 4, 1, MSG_NOERROR | IPC_NOWAIT);
			if(msg_status < 0)
			{
				perror ("Msgrcv hiba");
			}
			
			printf("Guests appeared: %s\nEvent success: %s\n", guests_appeared.names, Succ.success);
			memset(&guests_appeared.names[0], 0, sizeof(guests_appeared.names));
			memset(&Succ.success[0], 0, sizeof(Succ.success));
		}
			
		close(pipefd[1]);	
		msgctl(queue, IPC_RMID, NULL);
		printf("\n");
		
	}
	else if (child == 0)	
	{		
		printf("\n");	
		
		int i;
		for(i = 0; i < MAXCHILD; i++)
		{
			sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
				
			while(!usr_interrupt)
			{
                sigsuspend (&empty_sigmask);
			}
            
			usr_interrupt = 0;
				
			char helyszin[100];
			char nevek[1000];
				
			close(pipefd[1]);
			int read_status=read(pipefd[0], helyszin, sizeof(helyszin));
			
			if(read_status == -1)
			{
				perror("pipe hiba");
			}
			
			printf("Partner has arrived to location %s (#%d)\n", helyszin, i+1);
			memset(&helyszin[0], 0, sizeof(helyszin));	
											
			sigprocmask (SIG_BLOCK, &my_sigmask, NULL);
			kill(getppid(),SIGUSR2);
			while (!usr_interrupt)
			{
                    sigsuspend (&empty_sigmask);
			}
            usr_interrupt = 0;
								
			read_status = read(pipefd[0], nevek, sizeof(nevek));
			if(read_status == -1)
			{
				perror("pipe hiba");
			}
			
			printf("%s\n", nevek);
			
			char* p =  appeared(nevek);
							
			guests_appeared persons;
			persons.mtype = 1;
			strcpy(persons.names, p);
				
			int msg_status;
            msg_status = msgsnd(queue, &persons, strlen(persons.names), 0);
            
			if (msg_status < 0)
            {
                perror("msgsnd");
            }
								
			Success_Msg Succ;
			Succ.mtype = 1;
			sprintf(Succ.success, "%d\0", generate_number(1, 100));
				
			msg_status = msgsnd(queue, &Succ, strlen(Succ.success), 0);
			if (msg_status < 0)
            {
                perror("msgsnd");
            }

			free(p);
			memset(&nevek[0], 0, sizeof(nevek));
			kill(getppid(), SIGUSR2);
				
		}
		exit(0);	
		
	}
}



































