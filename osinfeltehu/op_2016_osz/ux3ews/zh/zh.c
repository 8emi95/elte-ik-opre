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

#define SIGSIKER SIGRTMIN + 1
#define SIGNEMSIKER SIGRTMIN + 2


struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
};

void handler(int signumber)
{
   if( signumber == SIGSIKER)
	   printf("Beertem \n");
   else
	   printf("Bevontattak \n");
   
   signal(signumber, handler);
}

int main(int argc, char** argv)
{
	
	signal(SIGSIKER,handler);
	signal(SIGNEMSIKER,handler);
	
	int uzenetsor, status; 
    key_t kulcs; 
	
	srand(time(NULL) + 100 * getpid());
	
	int osszszerelveny = atoi(argv[1]);
	int elromlottak = 0;
	
	kulcs = ftok(argv[0],1); // generalok egy kulcsot
    printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); //letrehozom az uzenetsort, ehhez kellett a kulcs
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
		return 1; 
    } 
	
	int i = 0;
	int is_parent = 0;
	while (i++ < osszszerelveny && is_parent == 0)
	{
		int parent = getpid();
		pid_t child = fork();
		if (child < 0)
		{
		printf("Error\n");
		return;
    }
    else if (child > 0)
    {
       // still in the parent;
	   int szerelvenyszam = rand() % osszszerelveny + 1;
		
		struct uzenet uz = { 1, "" };
		
		sprintf(uz.mtext, "%d", szerelvenyszam);

		
		struct uzenet uz2 = { 1, "" };
		sprintf(uz2.mtext, "%d", osszszerelveny);
		int status; 
     
		status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext) , 0 ); 
		if ( status < 0 ) 
         		 perror("msgsnd"); 
		status = msgsnd( uzenetsor, &uz2, sizeof(uz2.mtext) , 0 ); 
		if ( status < 0 ) 
         		 perror("msgsnd"); 
			 
		pause();
		struct uzenet uz3;
		status = msgrcv(uzenetsor, &uz3, 1024, 0, 0 ); 
		
		if ( status < 0 ) 
			perror("msgsnd"); 
		else
			printf( "Uzemelteto : a szerelveny valasza %s\n",  uz3.mtext );
		
		char str10[1000] = "Elcsusztam egy bananhelyon \n";
		if( strcmp(uz3.mtext, "Elcsusztam egy bananhelyon \n") == 0 )
		{
			pid_t potchild = fork();
			if (potchild < 0)
			{
				printf("Error\n");
				return;
			}
			else if (potchild > 0)
			{
				struct uzenet uz5;
				struct uzenet uz6;
				status = msgrcv(uzenetsor, &uz5, 1024, 0, 0 ); 
				if ( status < 0 ) 
					perror("msgsnd"); 
				status = msgrcv(uzenetsor, &uz6, 1024, 0, 0 ); 
						if ( status < 0 ) 
					perror("msgsnd"); 
				elromlottak = elromlottak + 1;
				printf("Potos: %s\n",uz5.mtext);
				printf("Potos: Jaratot helyettesitettem %s\n",uz6.mtext);
				printf("Elromlott szerelvenyek szama: %i\n",elromlottak);
				waitpid(potchild, NULL, 0);
			}
			else
			{
				printf("Potszerelveny\n");
				
				struct uzenet uz5 = { 1, "En vagyok a potszerelveny" };
				struct uzenet uz6 = { 1, "" };
				sprintf(uz5.mtext, "%d", i);
				int status; 
				status = msgsnd( uzenetsor, &uz5, sizeof(uz5.mtext) , 0 ); 
				if ( status < 0 ) 
					perror("msgsnd"); 
				status = msgsnd( uzenetsor, &uz6, sizeof(uz6.mtext) , 0 ); 
				if ( status < 0 ) 
					perror("msgsnd"); 
					
			}
		}
		
		waitpid(child, NULL, 0);
    }
	else
	{
       printf("child %i\n", i);
       is_parent = 1;
	   struct uzenet uz; 
			int status; 
			int answer;
			int szszam;
			int osszszam;

			status = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); 
     
			if ( status < 0 ) 
				perror("msgsnd"); 
			else
				printf( "Szerelveny1: A szerelveny szamom: %s\n",  uz.mtext );
			
			struct uzenet uz2; 

			status = msgrcv(uzenetsor, &uz2, 1024, 0, 0 ); 
     
			if ( status < 0 ) 
				perror("msgsnd"); 
			else
				printf( "Szerelveny1: A osszszerelveny szam: %s\n",  uz2.mtext );
			
			szszam = atoi(uz.mtext);
			osszszam = atoi(uz2.mtext);
			
			printf( "Szerelveny1: A hatralevo szerelvenyek szama: %i\n",  osszszam-szszam );
			
			if( rand()%10 +1 == 10)
			{
				printf( "Szerelveny1: Nem sikerult teljesitenem az uzemet \n" );
				kill(parent, SIGNEMSIKER);
				struct uzenet uz3 = { 1, "Elcsusztam egy bananhelyon \n" };
				status = msgsnd( uzenetsor, &uz3, sizeof(uz.mtext) , 0 ); 
				if ( status < 0 ) 
					perror("msgsnd"); 
			}
			else
			{
				printf( "Szerelveny1: Sikerult teljesitenem" );
				kill(getppid(), SIGSIKER);
				struct uzenet uz3 = { 1, "Minden rendben \n" };
				status = msgsnd( uzenetsor, &uz3, sizeof(uz.mtext) , 0 ); 
				if ( status < 0 ) 
					perror("msgsnd"); 
			}
    }
  }
	
}