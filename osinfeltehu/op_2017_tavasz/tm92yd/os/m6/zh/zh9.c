#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>

struct uzenet {
	long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	char mtext [ 1024 ];
};

void handler(int signumber){
	printf("HELLO ! (jelzes)\n",signumber);
}

int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 1, "Van-e kerdes?" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 

     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 1, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott kerdes szovege:  %s\n", uz.mtext ); 
     return 0; 
} 

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int uzenetsor, status;
	
	key_t kulcs;
    kulcs = ftok("temp",1);
	kulcs = ftok(argv[0],1);
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	if (uzenetsor < 0) {
		perror("msgget");
		return 1;
	}

	signal(SIGTERM, handler);

	pid_t child = fork();
	if (child > 0) {
        
		pid_t child2 = fork();
		if(child2 == 0) {
            printf("\tA masodik diak megerkezett az eloadasra\n");
            fogad(uzenetsor);
            sleep(2);
			kill(getppid(), SIGTERM);
            printf("A masodik diak tavozik az eloadasrol\n");
		} else {
            printf("A tanar felteszi a kerdest\n");

            kuld(uzenetsor);
            kuld(uzenetsor);

            int status;
			wait(NULL);
            wait(NULL);
			status = msgctl( uzenetsor, IPC_RMID, NULL );
			if ( status < 0 ) perror("msgctl");
			
			int status2;
			wait(&status2);
		    printf("A szulo befejezte a mukodeset\n");
		}
	} else {
        printf("\tAz elso diak megerkezett az eloadasra\n");
        fogad(uzenetsor);
        sleep(1);
		kill(getppid(), SIGTERM);
        printf("Az elso diak tavozik az eloadasrol\n");
	}
	return 0;
}
