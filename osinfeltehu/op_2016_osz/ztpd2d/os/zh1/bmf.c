#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
//#include <wait.h> 

#define START_MSG_TYPE 1
#define FINISH_MSG_TYPE 2

#define POT_SZERELVENY 1
#define RENDES_SZERELVENY 0

struct StartMsg {
	long mtype;
	
	int index;
	int max;
	int pot;
};

struct FinishMsg {
	long mtype;
	int status;
};

//
//      START MSG HANDLING
// 
int sendStartMsg( int uzenetsor, int index, int max ) {
	const struct StartMsg uz = { START_MSG_TYPE, index, max }; 
	int status; 

	status = msgsnd( uzenetsor, &uz, sizeof(uz), 0 ); 
	return status;
}

int rcvStartMsg( int uzenetsor, struct StartMsg* msg ) {
	int status = msgrcv(uzenetsor, msg, sizeof(*msg), START_MSG_TYPE, 0 ); 
	return status;
}


//
//      FINISH MSG HANDLING
// 
int sendFinishMsg( int uzenetsor, int success ) {
	const struct FinishMsg uz = { FINISH_MSG_TYPE, success }; 
	int status; 

	status = msgsnd( uzenetsor, &uz, sizeof(uz), 0 ); 
	return status;
}

int rcvFinishMsg( int uzenetsor, struct FinishMsg* msg ) {
	int status = msgrcv(uzenetsor, msg, sizeof(*msg), FINISH_MSG_TYPE, 0 ); 
	return status;
}


//
//      SZERELVENY INDITAS
// 
int indit( key_t kulcs, int index, int max, int tipus, int* success ) {
	pid_t child;
	int uzenetsor, status; 
	
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
	  perror("msgget"); 
	  return 1; 
	} 
	
	child = fork(); 
    if ( child > 0 ) {
		
		// PARENT
		// SZERELVENY INDITAS
		printf("[parent] Szerelveny inditasa..\n");
		status = sendStartMsg(uzenetsor, index, max);
		if( status < 0 ) {
			perror("msgsnd"); 
			return status;
		}
		
		
		// SZERELVENY UZEM VEGE
		struct FinishMsg finMsg;
		status = rcvFinishMsg(uzenetsor, &finMsg);
		if( status < 0 ) {
			perror("msgget"); 
			return status;
		}
		
		*success = finMsg.status;
		if( *success > 0 ) {
			printf("[parent] Szerelveny sikeresen vegzett.\n");
		} else {
			printf("[parent] Szerelvenyt bevontattak. (Hibakod: %d)\n", *success);
		}
		
		
		wait();
		
	} else {
		
		// CHILD
		// SZERELVENY INDULASA
		struct StartMsg msg;
		status = rcvStartMsg(uzenetsor, &msg);
		if( status >= 0 ) {
			if( tipus == RENDES_SZERELVENY ) {
				printf("[szerelveny] %d. szerelveny (osszesen: %d) elindult.\n", msg.index, msg.max);
			} else {
				printf("[szerelveny] POTSZERELVENY! Helyettesites: %d. szerelveny (osszesen: %d) elindult.\n", msg.index, msg.max);
			}
		} else {
			perror("msgget");
			return status;
		}
		
		// SZERELVENY KOZLEKEDIK
		srand(time(NULL)*(getppid() + getpid())); 
		int p=rand()%100;
		int success = 1;
		if( p < 10 ) {
			success = -1 * rand()%42;
		}
		
		// SZERELVENY VEGZETT
		if( success > 0 ) {
			printf("[szerelveny] Uzem sikeresen teljesitve.\n");
		} else {
			printf("[szerelveny] Uzem nem teljesitett...\n");
		}
		sendFinishMsg(uzenetsor, success);
		
		exit(0);
	}
}

int main (int argc, char* argv[]) {
	
	if(argc < 2 ) { 
		printf("Max szerelveny parameter hianyzik..\n");
		return 0;
	}
	
	int max = atoi(argv[1]);
	
	key_t kulcs;
	kulcs = ftok(argv[0],1);
	
	int i;
	int mentesitoCount = 0;
	for( i = 0; i < max; ++i ) {
		int vegigment;
		indit(kulcs, i+1, max, RENDES_SZERELVENY, &vegigment);
		if(vegigment < 0) {
			printf("\n");
			++mentesitoCount;
			indit(kulcs, i+1, max, POT_SZERELVENY, &vegigment);
		}
		printf("\n");
		
	}
	
	printf("Osszesen %d mentesito szerelvenyt kellett inditani.\n", mentesitoCount);
	return 0;
}