#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

#define MAX_SIZE 1000

void handler(int signal) {
	
}


int fd[2];

struct msg {
	long mtype;
	pid_t telefon;
	char lakcim[MAX_SIZE];
};


int main(int argc, char ** argv) {
	char lakcim[MAX_SIZE];
	if(argc < 2) {
		printf("nincs lakcim\n");
		return 1;
	}
	strcpy(lakcim,argv[1]);
	
	if(pipe(fd) < 0) {
		printf("rossz a cso\n");
		return 1;
	}
	
	int uzenetsor;
	key_t kulcs;
	kulcs = ftok(argv[0],1);
	uzenetsor = msgget(kulcs,0600 | IPC_CREAT);
	if ( uzenetsor < 0 ) { 
		printf("uzenetsor hiba");
        return 1; 
    }
	
	pid_t utas = fork();
	signal(SIGUSR1,handler);
	if(utas > 0) {
		//szulo
		pause();
		close(fd[1]);
		char utas_lakcime[MAX_SIZE];
		read(fd[0],utas_lakcime,MAX_SIZE);
		printf("KOZPONT: Az utas lakcime: %s\n",utas_lakcime);
		pid_t taxis = fork();
		if(taxis > 0) {
			//szulo
			struct msg uz;
			uz.mtype = 1;
			uz.telefon = utas;
			stpcpy(uz.lakcim,utas_lakcime);
			msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
			printf("KOZPONT: lakcim elkuldve a taxisnak\n");
			kill(taxis,SIGUSR1);
			pause();
			struct msg kapott;
			msgrcv(uzenetsor,&kapott,sizeof(kapott),1,0);
			printf("KOZPONT: a TAXIS uzeni hogy: %s\n",kapott.lakcim);
			msgctl( uzenetsor, IPC_RMID, NULL );
			wait(NULL);
			wait(NULL);
			
		} else if(taxis == 0) {
			struct msg kapott;
			pause();
			msgrcv(uzenetsor,&kapott,sizeof(kapott),1,0);
			printf("TAXIS: %i\t%s\n",kapott.telefon,kapott.lakcim);
			struct msg uz;
			uz.mtype = 1;
			strcpy(uz.lakcim,"ELINDULTAM");
			msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
			kill(getppid(),SIGUSR1);
		}
		
		
	} else if(utas == 0) {
		//utas
		printf("UTAS: kuldom a lakcimet: %s\n",lakcim);
		close(fd[0]);
		write(fd[1],lakcim,MAX_SIZE);
		close(fd[1]);
		kill(getppid(),SIGUSR1);
	}
	
	
	
	return 0;
}