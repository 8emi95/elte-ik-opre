#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define MEMSIZE 1024

struct message
{
	long mtype;
	int mtext;
};

int send(int message_queue, int mtext)
{
	const struct message msg = {5, mtext};
	int status = msgsnd(message_queue, &msg, sizeof(msg.mtext), 0);
	if (status < 0)
	{
		perror("msgsnd");
		return -1;
	}
	
	return 0;
}

int receive(int message_queue)
{
	struct message msg;
	int status = msgrcv(message_queue, &msg, sizeof(msg.mtext), 5, 0);
	
	if (status < 0)
	{
		perror("msgrcv");
		return -1;
	}
	
	return msg.mtext;
}

void handler(int signumber) {}

int szemafor_letrehozas(const char* pathname,int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}


void szemafor_muvelet(int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if(semop(semid,&muvelet,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void szemafor_torles(int semid){
      semctl(semid,0,IPC_RMID);
}

int main(int argc, char* argv[])
{
	int message_queue, status;
	key_t key;
	
	key = ftok(argv[0], 15);
	if (key < 0)
	{
		perror("ftok");
		return -1;
	}
	
	message_queue = msgget(key, 0600 | IPC_CREAT);
	if (message_queue == -1)
	{
		perror("msgget");
		return -1;
	}
	
    int oszt_mem_id=shmget(key,500,IPC_CREAT|S_IRUSR|S_IWUSR);
	char* s = shmat(oszt_mem_id,NULL,0);

	signal(SIGUSR1, handler);
	
    int semid = szemafor_letrehozas(argv[0],1);

	pid_t child = fork();
	if (child > 0)
	{
		srand(time(NULL));
		int osszes_szerelveny = atoi(argv[1]);
		// struct message msg = {5, osszes_szerelveny};
		
		int i;
		int potok = 0;
		int total_time = 0;
		int successful = 0;
		for (i = 1; i <= osszes_szerelveny; ++i)
		{
			send(message_queue, 0);
			send(message_queue, i);
			
			// msg.mtext = atoi(argv[1]);
			send(message_queue, osszes_szerelveny);
			
			pause();
			
			int cause = receive(message_queue);
			
			switch (cause)
			{
				case -1:
					printf("A(z) %i. szerelvényt elvontatták, mert elcsúszott egy banánhéjon.\n", i);
					
					break;
				case -2:
					printf("A(z) %i. szerelvényt elvontatták, mert lefagyott.\n", i);
					break;
				default:
					break;
			}
			
			sleep(1);
			int time;
			szemafor_muvelet(semid,-1); // down, wait if necessary
			strcpy((char*)&time, s);
			szemafor_muvelet(semid,1); // up      
			sleep(1);
			szemafor_muvelet(semid,-1); // down, wait if necessary
			strcpy((char*)&time, s);
			szemafor_muvelet(semid,1); // up      
			
			if (time > 0)
			{
				total_time += time;
				++successful;
			}
			
			if (cause < 0)
			{
				++potok;
				send(message_queue, 1);
				send(message_queue, i);
				
				// msg.mtext = atoi(argv[1]);
				send(message_queue, osszes_szerelveny);
				
				pause();
				
				int cause = receive(message_queue);
				
				switch (cause)
				{
					case -1:
						printf("A(z) %i. szerelvényt elvontatták, mert elcsúszott egy banánhéjon.\n", i);
						
						break;
					case -2:
						printf("A(z) %i. szerelvényt elvontatták, mert lefagyott.\n", i);
						break;
					default:
						break;
				}
				
				sleep(1);
				int time;
				szemafor_muvelet(semid,-1); // down, wait if necessary
				strcpy((char*)&time, s);
				szemafor_muvelet(semid,1); // up      
				sleep(1);
				szemafor_muvelet(semid,-1); // down, wait if necessary
				strcpy((char*)&time, s);
				szemafor_muvelet(semid,1); // up      
				
				if (time > 0)
				{
					total_time += time;
					++successful;
				}
			}
		}
		
		printf("\nMa %i darab mentesítő járatot kellett indítani.\n", potok);
		
		printf("A mai átlagos utazási idő: %f perc.\n", ((double)total_time) / successful);
		
		// wait(NULL);
		status = msgctl(message_queue, IPC_RMID, NULL);
		if (status < 0)
		{
			perror("msgctl");
			return -1;
		}

		shmdt(s);	   
		szemafor_torles(semid);
		shmctl(oszt_mem_id,IPC_RMID,NULL);
	}
	else if (child == 0)
	{
		srand(time(NULL));
		
		while (1)
		{
			int potvagyoke = receive(message_queue);
			int my_number = receive(message_queue);
			int osszes_szerelveny = receive(message_queue);
			
			if (potvagyoke)
				printf("Pótszerelvény vagyok, a(z) %i. járatot helyettesítem.\n", my_number);
			else
				printf("A(z) %i. szerelvény vagyok, utánam még %i darab lesz.\n", my_number, osszes_szerelveny - my_number);
				
			int cause;
			switch (rand() % 20)
			{
				case 0:
				{
					puts("Elcsúsztam egy banánhéjon!");
					cause = -1;
				}
					break;
				case 1:
				{
					puts("Lefagytam a zord télben!");
					cause = -2;
				}
					break;
				default:
				{
					puts("Teljesítettem az üzemet.");
					cause = 0;
				}
			}

			send(message_queue, cause);
			
			sleep(1);
			kill(getppid(), SIGUSR1);
			
			szemafor_muvelet(semid,-1); // down, wait if necessary
			strcpy(s, (const char*)&my_number);
			szemafor_muvelet(semid,1); // up      
			sleep(1);
			int time = cause == 0 ? rand() % 21 + 10 : -1;
			szemafor_muvelet(semid,-1); // down, wait if necessary
			strcpy(s, (const char*)&time);
			szemafor_muvelet(semid,1); // up      
			sleep(1);
			
			if (my_number == osszes_szerelveny && (cause == 0 || potvagyoke)) break;
		}
		
		shmdt(s);	   
	}
	else
	{
		perror("fork");
		return -1;
	}
}
