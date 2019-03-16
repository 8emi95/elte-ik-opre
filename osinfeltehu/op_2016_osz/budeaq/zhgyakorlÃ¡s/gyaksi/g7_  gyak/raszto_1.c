#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork, for pipe()
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <sys/time.h>
//ipcs parancs
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <string.h> 
#include <wait.h> 

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include <signal.h>

void handler(int signumber){
	//printf("Signal with number %i has arrived\n",signumber);
	//signal(signumber,SIG_DFL);
	//raise(signumber);
	//signal(signumber,SIG_IGN);
}

struct uzenet {
	long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	char mtext [ 1024 ];
};

// sendig a message
int kuld( int uzenetsor, int jaror)
{
	const char* jokivansag[] = {"1 Jo reggelt!", "2 Jo reggelt!", "3 Jo reggelt!", "4 Jo reggelt!", "5 Jo reggelt!", "6 Jo reggelt!"};
	int idx = rand()%6;
	//printf("az index: %i %s\n", idx, jokivansag[idx]);
	struct uzenet uz;
	uz.mtype = jaror; // 1. uzenet tipusa
	sprintf(uz.mtext,"%s", jokivansag[idx]);
	//printf("az index: %i %s\n", idx, uz.mtext);
	printf("A %i. jarornek kuldott uzenet: %s\n", jaror, uz.mtext);
	
	//const struct uzenet uz = { 5, "Hajra Fradi!" }; // 1. uzenet tipusa
	int status;

     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); // 1. azon, 2. uzenet cime, 3.. uzenet hossza ,1 nulla karakter is kell, 4.
        // a 3. param ilyen is lehet: sizeof(uz.mtext)
        // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 )
          perror("msgsnd");
     return 0;
}

// receiving a message. 
int fogad( int uzenetsor, int jaror )
{
     struct uzenet uz;
     int status;
     // az utolso parameter(0) az uzenet azonositoszama
        // ha az 0, akkor a sor elso uzenetet vesszuk ki
        // ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
        // vesszuk ki a sorbol 
     //status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); // ha 5 helyet 0, mindenfele uzenetet beolvas
     status = msgrcv(uzenetsor, &uz, 1024, jaror, 0 ); // ha 5 helyet 0, mindenfele uzenetet beolvas

     if ( status < 0 )
          perror("msgsnd");
     else
          //printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
          printf( "A %i. jaror uzenete:  %s\n", jaror, uz.mtext );
     return 0;
}

int main(int argc, char* argv[])
{
	// ez cikluson kivul legyen!!!!
	srand(time(NULL)); //the starting value of random number generation

	//** signal
	signal(SIGTERM, handler);

	//** uzenetsor
        int uzenetsor, msgStatus;
        key_t kulcs;

        // msgget needs a key, amelyet az ftok generál 
        //. 
        kulcs = ftok(argv[0],1);  // 2. par hanyadik kulcsot hozzuk letre, ftok egyedi kulcs
        //printf ("A kulcs: %d\n",kulcs);
        uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );  // IPC_PRIVATE
        if ( uzenetsor < 0 ) {
                perror("msgget");
                return 1;
        }

	//** fork
	int status;
	pid_t  rendk1=fork(); //forks make a copy of variables
	if (rendk1<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (rendk1>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		pid_t  rendk2=fork(); //forks make a copy of variables
		if (rendk2<0){perror("The fork calling was not succesful\n"); exit(1);} 
		// kozpont
		if (rendk2>0) //the parent process, it can see the returning value of fork - the child variable!
		{
			pause(); //waits till a signal arrive 
			pause();
			//printf("kozpont\n");
			printf("A jarorok indulasra keszek!\n\n");

			kuld( uzenetsor, 2 ); //2esnek
			//kill(rendk2, SIGTERM);
			//pause();
			kuld( uzenetsor, 1 ); //1esnek
			//kill(rendk1, SIGTERM);

			waitpid(rendk2,&status,0); 
			waitpid(rendk1,&status,0); 
			printf("A kozpont befejezte!\n");
		}
		else { //jaror 2
			//printf("jaror 2\n");
			printf("A jaror 2 elindult!\n");
			kill(getppid(), SIGTERM);
			kill(rendk1, SIGTERM);

			// eloszor a 2es kap jo kivansagot!
			//sleep(3);
			////pause();
			//printf("jaror 2 masodjara\n");
			fogad(uzenetsor, 2);			
			////kill(getppid(), SIGTERM);
			////kill(rendk1, SIGTERM);

			printf("Jaror 2 befejezte!\n\n");
			return 0;
		}
	}
	else //jaror 1 //child process
	{
		pause();
		//printf("jaror 1\n");
		printf("A jaror 1 elindult!\n");
		kill(getppid(), SIGTERM);

		// eloszor a 2es kap jo kivansagot!
		////pause();
		////pause();
		//printf("jaror 1 masodjara\n");
		fogad(uzenetsor, 1);

		printf("Jaror 1 befejezte!\n\n");
		return 0;
	}

	return 0;
	//exit(EXIT_SUCCESS);	// force exit, not necessary
}
