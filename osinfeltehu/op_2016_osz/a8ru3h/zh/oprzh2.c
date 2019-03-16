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
	int teljesitette;
	srand(time(NULL)+getpid());
	hanyadik_vagyok = fogadint( uzenetsor, 1 ); 
	hanyan_vagyunk = fogadint( uzenetsor, 1 ); 
	teljesitette = (rand()%100 < 90);
	printf("Ennyiedik vagyok: %d\n", hanyadik_vagyok);
	printf("Ennyien vannak még: %d\n", hanyan_vagyunk-hanyadik_vagyok);
	if (teljesitette) {
		printf("Az üzemet teljesítette\n");
	} else {
		printf("Az üzemet sajnos nem sikerült teljesíteni\n");
	}
	sleep(1);
	kill(uzemelteto, SIGUSR1);
	const struct intuzenet uz = { 2, teljesitette }; 
	kuldint( uzenetsor, uz );  // Parent sends a message. 
}

void handler(int signumber) {
	printf("A szerelvény beérkezett az állomásra...\n");
}

int main (int argc, char* argv[]) { 
	pid_t child; 
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
	child = fork(); 
	if ( child > 0 ) { 
		int sajat_labon;
		signal(SIGUSR1, handler);
		srand(time(NULL)+getpid());
		hanyadik_szerelveny = rand() % szerelvenyek_szama + 1;
		const struct intuzenet uz = { 1, hanyadik_szerelveny }; 
		kuldint( uzenetsor, uz );  // Parent sends a message. 
		const struct intuzenet uz2 = { 1, szerelvenyek_szama }; 
		kuldint( uzenetsor, uz2 );
		pause();
		sajat_labon = fogadint( uzenetsor, 2 ); 
		if (sajat_labon) {
			printf("...saját lábon\n");
		} else {
			printf("...vontatva\n");
		}
	} else if ( child == 0 ) {
		szerelveny(uzenetsor, getppid());
		return 0;
	} else { 
		perror("fork"); 
		return 1; 
	} 

	return 0; 
} 
