#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void handler(int signumber)
{
   ;
}

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 1, "Kerdes?" };
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     
	 return 0; 
} 

sem_t* szemafor_letrehozas(char*nev,int szemafor_ertek)
{
    sem_t *semid=sem_open(nev,O_CREAT,S_IRUSR|S_IWUSR,szemafor_ertek );
	if (semid==SEM_FAILED)
	perror("sem_open");
       
    return semid;
}

void szemafor_torles(char* nev)
{
      sem_unlink(nev);
}

int main(int argc, char** argv)
{
     int uzenetsor, status; 
     key_t kulcs; 
	 
	 key_t osztottmemokulcs;
	 int oszt_mem_id;
	 
	  int pipefdf[2]; // unnamed pipe file descriptor array
	  int pipefds[2]; // unnamed pipe file descriptor array
	  
	  char sz1[100];
	  char sz2[100];
	  	  
		 char *s;
	  signal(SIGUSR1,handler);
	  signal(SIGUSR2,handler);
	  
	  
	  
	  oszt_mem_id=shmget(osztottmemokulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR); // osztott memória létrehozása
	  s = shmat(oszt_mem_id,NULL,0); // memória rész hozzárendelés
	  
	  char* sem_nev="/alma1";
	  sem_t* semid;
	  
	  semid = szemafor_letrehozas(sem_nev,0);
	  
    if (pipe(pipefdf) == -1) 
	{
		perror("Hiba az elso pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefds) == -1) 
	{
		perror("Hiba az masodik pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	 
	 kulcs = ftok(argv[0],1); // generalok egy kulcsot
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); //letrehozom az uzenetsort, ehhez kellett a kulcs
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

	pid_t child1 = fork();
	if(child1 < 0)
	{
		printf("Error.\n");
	}
	else if (child1 > 0)
	{
		pid_t child2 = fork();
		if (child2 < 0)
		{ 
		   printf("Error\n");
		}
		else if (child2 > 0)
		{
			//printf("Parent.\n");
			kuld( uzenetsor );
			
			printf("Eloado: Varom a valaszokat!\n");
			
			int answers = 0;
			int answerf;
			
			close(pipefdf[1]); // Az iro részre nincs szükség azt bezárjuk
			close(pipefds[1]); 
						
			pause();
			read(pipefdf[0], &answerf, sizeof(int));
			printf("Eloado: Az elso hallgato valasza: %i\n", answerf);
			pause();
			read(pipefds[0], &answers, sizeof(int));
			printf("Eloado: A masodik hallgato valasza: %i\n", answers);
			
			close(pipefdf[0]); // Már az olvasó részre sincs szükség ezt is bezárom
			close(pipefds[0]);
			
			/*  kritikus szakasz kezdete   */
			printf("Eloado: Indul a posix szemafor down!\n");
       		sem_wait(semid);	// semaphore down
       		printf("Eloado: Down rendben, eredmeny: %s",s);  
       		sem_post(semid);      
       		/*  kritikus szakasz vége  */  	
			
			shmdt(s);
			
			szemafor_torles(sem_nev);
            shmctl(oszt_mem_id, IPC_RMID, NULL);
			
			waitpid(child1, NULL, 0);
			waitpid(child2, NULL, 0);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
		   printf("Eloado: Befejeztem\n");
          return 0; 
		   
		}
		else
		{
			//printf("Child 2\n");
			struct uzenet uz; 
			int status; 
			int answer;
	
			srand(time(NULL) + 100 * getpid());
			status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
			if ( status < 0 ) 
				perror("msgsnd"); 
			else
				printf( "Hallgato2: Az eloado kerdese: %s\n",  uz.mtext );
			answer = rand()%4 +1;
			sleep(2);
			printf( "Hallgato2: Az en valaszom: %d\n",  answer );
			close(pipefds[0]);  //Usually we close the unused write end
			write(pipefds[1], &answer, sizeof(int) );
			close(pipefds[1]);  //Usually we close the unused write end
			kill(getppid(), SIGUSR2);
			
			if( (rand()%2+1) == 1 )
			{
				char buffer[] = " A masodik hallgato nem erti\n";
				strcpy(s,buffer);
				shmdt(s); // Up	shmdt(s);	// elengedjük az osztott memóriát
				shmctl(oszt_mem_id,IPC_RMID,NULL);
			}
			sem_post(semid); // Up
			
			
			
			sleep(2);
			printf( "Hallgato2: befejeztem\n" );
			return 0; 

		   
		}
	}
	else
	{
		//printf("Child 1\n");
		struct uzenet uz; 
		int status; 
		int answer;
		srand(time(NULL) + 100 * getpid());
		status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
		if ( status < 0 ) 
			perror("msgsnd"); 
		else
			printf( "Hallgato1: Az eloado kerdese: %s\n",  uz.mtext );
		answer = rand()%4 +1;
		printf( "Hallgato1: Az en valaszom: %d\n",  answer );
		close(pipefdf[0]);  //Usually we close the unused write end
		write(pipefdf[1], &answer, sizeof(int) );
		close(pipefdf[1]);  //Usually we close the unused write end
		kill(getppid(), SIGUSR1);
		
		if( (rand()%2+1) == 1 )
		{
			char buffer[] = " AZ elso hallgato nem erti\n";
			strcpy(s,buffer);
			shmdt(s);	// elengedjük az osztott memóriát
			shmctl(oszt_mem_id,IPC_RMID,NULL);
		}
		sem_post(semid); // Up
			
		sleep(2);
		printf( "Hallgato1: befejeztem\n" );
		return 0; 
			
	}
  return 0;
}

/*
int msgrcv(int msgid, void *addr, size_t nbytes, long msgtype, int flg);
	msgtype = 0  –  a sor elsõ üzenetét olvassa ki,
	msgtype > 0  –  a sor elsõ msgtype típusú üzenetét olvassa ki,
	msgtype < 0  –  a sor legkisebb, de az abs(msgtype)-nál nagyobb vagy azzal egyenlõ értékû üzenetét olvassa ki.
*/

/*read-elni a [0]-n write-olni az [1]n*/