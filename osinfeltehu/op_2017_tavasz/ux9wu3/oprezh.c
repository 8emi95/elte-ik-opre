#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
int stillWaiting = true;
int stillWaiting2 = true;

void handler(int signum)
{
  stillWaiting = false;
}
void handler2(int signum)
{
  stillWaiting2 = false;
}
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
}

int main(int argc, char**argv)
{
	int status;
	int toParent[2];
	pipe(toParent);
	char lakcim[100];
	pid_t  child1=fork();
	if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
	
	if (child1>0) //the parent process
	{
		//uzenetsoros dolgok
		int uzenetsor;
		key_t kulcs = ftok("ux9wu3",1); 
		uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
		
		close(toParent[1]);
		pid_t  child2=fork();
		if (child2 > 0) //taxi társaság
		{
			while(stillWaiting)
			{
				usleep(1);
			}
			//A
			read(toParent[0], lakcim, sizeof(lakcim));
			printf("Utas cime a kozpontban:%s\n", lakcim);
			
			//B
			struct uzenet uz;
      strcpy(uz.mtext, lakcim);
      uz.mtype = 5;
			int status; 
 
      printf("Kozpontban az uatas telefonja: %d\n", &child1);
      
      //lakcim kuldese
			status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
      
      
      //telefon kuldese
      memcpy(uz.mtext, &child1, sizeof(pid_t));
			status = msgsnd( uzenetsor, &uz, sizeof(pid_t) , 0 ); 
			
			//valasz
      //sleep(1);
      while (stillWaiting2)
      {
        usleep(1);
      }
			status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
      printf("Taxis valasza: %s\n", uz.mtext);

			waitpid(child1,&status,0);
			waitpid(child2,&status,0); 
			msgctl(uzenetsor, IPC_RMID, NULL);
		} else //taxis
		{
			struct uzenet uz; 
      uz.mtype = 5;
      //cim
			int status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
			printf("Utas cime a taxisnal: %s\n", uz.mtext);
      
      //telefon
			status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
      pid_t utas;
      memcpy(&utas, uz.mtext, sizeof(pid_t));
      printf("Taxisnal az utas telefonja: %d\n", &utas);

      //resp/kill

      kill(getppid(), SIGUSR2);
      char respmsg[] = "Elindultam az utasert!";
      strcpy(uz.mtext, respmsg);
			status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
			
		}
		close(toParent[0]);
	}
	else //ügyfél
	{
		close(toParent[0]);
		kill(getppid(), SIGUSR1);
		write(toParent[1], argv[1], sizeof(argv[1]));
		close(toParent[1]);
	}
	return 0;
}
