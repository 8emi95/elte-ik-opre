#include <sys/ipc.h>
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct intuzenet {
	long mtype;
	int mvalue;
};

int kuldint( int uzenetsor, struct intuzenet uz ) 
{ 
	int status; 

	status = msgsnd( uzenetsor, &uz, sizeof ( int ) , 0 ); 
	if ( status < 0 ) 
		perror("msgsnd"); 
	return 0; 
} 
     
int fogadint( int uzenetsor, int n ) 
{ 
	struct intuzenet uz; 
	int status; 
	status = msgrcv(uzenetsor, &uz, 1024, n, 0 ); 

	if ( status < 0 ) 
		perror("msgsnd"); 
	return uz.mvalue;
} 

void szerelveny(int uzenetsor, pid_t uzemelteto) {
	int hanyadik_vagyok;
	int hanyan_vagyunk;
	int pot_e;
	int teljesitette;
	srand(time(NULL)+getpid());
	hanyadik_vagyok = fogadint( uzenetsor, 1 ); 
	hanyan_vagyunk = fogadint( uzenetsor, 1 ); 
	pot_e = fogadint( uzenetsor, 3 ); 
	teljesitette = (rand()%100 < 90);
	if (pot_e == 0) {
		printf("Ennyien vannak még: %d\n", hanyan_vagyunk-hanyadik_vagyok);
		printf("Ennyiedik vagyok: %d\n", hanyadik_vagyok);
		printf("Menetrend szerinti járat vagyok\n");
	} else {
		printf("Pótjárat vagyok e-helyett: %d\n", pot_e);
	}
	if (teljesitette) {
		printf("Az üzemet teljesítettem\n");
	} else {
		printf("Az üzemet sajnos nem sikerült teljesíteni\n");
	}
	sleep(1);
	kill(uzemelteto, pot_e==0 ? SIGUSR1 : SIGUSR2);
	const struct intuzenet uz = { 2, teljesitette }; 
	kuldint( uzenetsor, uz );  // Parent sends a message. 
}

int napi_mentesitok_szama = 0;

void handler(int signumber) {
	printf("A szerelvény beérkezett az állomásra...\n");
}

void handler2(int signumber) {
	printf("A mentesítő szerelvény beérkezett az állomásra...\n");
	napi_mentesitok_szama++;
}

int main (int argc, char* argv[]) { 
	pid_t child, child2; 
	int uzenetsor, status; 
	key_t kulcs;
	int szerelvenyek_szama;
	int hanyadik_szerelveny;

	if (argc < 2) {
		printf("Használat: %s <szerelvények száma>\n\n", argv[0]);
		return 1;
	}
	szerelvenyek_szama = atoi (argv[1]);

	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	} 

	int i;
	for (i=1; i<=szerelvenyek_szama; i++) {
		child = fork(); 
		if ( child > 0 ) { 
			int sajat_labon;
			signal(SIGUSR1, handler);
			srand(time(NULL)+getpid());
			hanyadik_szerelveny = i;
			const struct intuzenet uz = { 1, hanyadik_szerelveny }; 
			kuldint( uzenetsor, uz );  // Parent sends a message. 
			const struct intuzenet uz2 = { 1, szerelvenyek_szama }; 
			kuldint( uzenetsor, uz2 );
			const struct intuzenet uz3 = { 3, 0 }; 
			kuldint( uzenetsor, uz3 );
			pause();
			sajat_labon = fogadint( uzenetsor, 2 ); 
			if (sajat_labon) {
				printf("...saját lábon\n");
			} else {
				printf("...vontatva, megy a mentesítő\n");
				child2 = fork(); 
				if ( child2 > 0 ) { 
					int sajat_labon;
					signal(SIGUSR2, handler2);
					srand(time(NULL)+getpid());
					hanyadik_szerelveny = i;
					const struct intuzenet uz4 = { 1, hanyadik_szerelveny }; 
					kuldint( uzenetsor, uz4 );  
					const struct intuzenet uz5 = { 1, szerelvenyek_szama }; 
					kuldint( uzenetsor, uz5 );
					const struct intuzenet uz6 = { 3, i }; 
					kuldint( uzenetsor, uz6 );
					pause();
					sajat_labon = fogadint( uzenetsor, 2 ); 
					if (sajat_labon) {
						printf("...saját lábon\n");
					} else {
						printf("...vontatva, ez egy ilyen nap\n");
					}
					int waitstatus;
					waitpid(child2, &waitstatus, 0);
				} else if ( child2 == 0 ) {
					szerelveny(uzenetsor, getppid());
					exit(0);
				} else { 
					perror("fork"); 
					return 1; 
				} 
			}
			int waitstatus;
			waitpid(child, &waitstatus, 0);
		} else if ( child == 0 ) {
			szerelveny(uzenetsor, getppid());
			exit(0);
		} else { 
			perror("fork"); 
			return 1; 
		} 
	} 
	printf("A mai napon ennyi mentesítőt indítottunk: %d\n", napi_mentesitok_szama);
	status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
	return 0; 
} 
