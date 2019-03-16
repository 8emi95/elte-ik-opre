#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MSGSIZE 40
#define MAXMSGS 5
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1023 ]; 
}; 

int kuld( int uzenetsor, struct uzenet * uz) 
{ 
     int status; 
     status = msgsnd( uzenetsor, uz, strlen( uz->mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

void helyszinreErkezett(int signum) {
	printf("Gyerek megerkezett a vegallomasra\n");
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
          printf( "%s\n", uz.mtext ); 
     return atoi(uz.mtext); 
} 

int main(int argc, char **argv) {
	pid_t parent_pid = getpid();
	int i = 0;
		int status;
		struct mq_attr attr;
		attr.mq_maxmsg = MAXMSGS;
		attr.mq_msgsize = MSGSIZE;
		key_t kulcs;
		kulcs = ftok(argv[0],1);
		int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
		if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
		}
		id_t pid;
		if ((pid = fork()) == -1) {
			printf("fork hiba");
			exit(1);
		} else if(pid == 0) {
			srand(time(NULL));
			int siker = rand()%100;
			struct uzenet uz;
			uz.mtype = 5;
			stpcpy(uz.mtext, "sikeresen");
			if(siker < 10){
				uz.mtype = 5;
				stpcpy(uz.mtext, "banan");
			}
			printf("Gyerek: Jaratszama:\n");
			fogad(uzenetsor);
			printf("Gyerek: osszes jarat:\n");
			int szam = fogad(uzenetsor);
			printf("Gyerek: meg ennyi van vissza: %d\n", szam-1);
			kill(parent_pid, SIGUSR1);
			
			kuld(uzenetsor, &uz);
			exit(1);
		} else {
			sigset_t set;
			sigfillset(&set);
			signal(SIGUSR1, helyszinreErkezett);
			sigdelset(&set, SIGUSR1);
			struct uzenet uz = {5, "1"};
			kuld(uzenetsor, &uz);
			stpcpy(uz.mtext, argv[1]);
			kuld(uzenetsor, &uz);
			sigsuspend(&set);
			fogad(uzenetsor);
		}
	exit(1);
}