#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int stillWaiting = true;

void handler(int signum)
{
  stillWaiting = false;
}

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 


int main(int argc, char**argv)
{
  if (argc != 2) return 1;
	int status;
	int toParent[2];
	pipe(toParent);
	char lakcim[100];
	pid_t  child1=fork();
	if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 
	signal(SIGUSR1, handler);
	
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
      uz.mtype = 1;
			int status; 
      
      
      printf("Kozpontban az uatas telefonja: %x\n", child1);
      

      pid_t child1_2;
      char buf[100];
      memcpy(buf, &child1, sizeof(child1));
      memcpy(&child1_2, buf, sizeof(child1_2));
      printf("Kozpontban az uatas telefonja másolás után: %x\n", child1_2);


      //lakcim kuldese
			status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
      
      
      //telefon kuldese
      memcpy(uz.mtext, &child1, sizeof(pid_t));
			status = msgsnd( uzenetsor, &uz, sizeof(pid_t) , 0 ); 
			
			//valasz
      //sleep(1);
			status = msgrcv(uzenetsor, &uz, 1024, 2, 0 );
      printf("Taxis valasza: %s\n", uz.mtext);

			waitpid(child1,&status,0);
			waitpid(child2,&status,0); 
			msgctl(uzenetsor, IPC_RMID, NULL);
		} else //taxis
		{
			struct uzenet uz; 
      //cim
			int status = msgrcv(uzenetsor, &uz, 1024, 1, 0 );
			printf("Utas cime a taxisnal: %s\n", uz.mtext);
      
      //telefon
			status = msgrcv(uzenetsor, &uz, 1024, 1, 0 );
      pid_t utas;
      memcpy(&utas, uz.mtext, sizeof(pid_t));
      printf("Taxisnal az utas telefonja: %x\n", utas);

      //resp/kill

      char respmsg[] = "Elindultam az utasert!";
      uz.mtype = 2;
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
