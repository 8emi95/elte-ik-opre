#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

#include <mqueue.h>
#include <errno.h>

#include <sys/time.h>

#include <signal.h>
#include <sys/types.h>

#define MAX_SZERELVENY 10

void handler(int signumber){
  printf("Elcsuszott egy bananhejon!\n",signumber);
}

struct Kerdes {
	int hanyadik;
	int osszesen;
	char * uzenet[10];
};

int main() {
	
	signal(SIGRTMIN,handler);
	
	// -------------------- MsgQueue --------------
	char* msg_queue_name = "/kerdes";
	mqd_t msg_queue_id;
	struct mq_attr msg_queue_attrib;

	msg_queue_attrib.mq_maxmsg = 10; // Nem enged sokat :(
	msg_queue_attrib.mq_msgsize = sizeof(struct Kerdes);
	mq_unlink(msg_queue_name); // biztos, ami biztos...

	msg_queue_id = mq_open(msg_queue_name, O_CREAT | O_RDWR, 0600, &msg_queue_attrib);
	if ( msg_queue_id < 0 ) {
		printf("MsgQueue open Hiba : %d\n", errno);
		return;
	}
	
	// -------------------- MsgQueue --------------
	
	
	int status;
	int notacommonvalue = 1;
	printf("The value is %i before forking \n", notacommonvalue);
	
//---------   FORK   ---------
	
	pid_t metro = fork();
	if (metro < 0) { 
		perror("The fork calling was not succesful\n");
		exit(1);
	}
	
	if (metro > 0) {
//---------   parent -> BMF   ---------
		
		// -------------------- MsgQueue --------------
		srand(time(NULL));
		int r = rand() % MAX_SZERELVENY;
		struct Kerdes kerdes;
			kerdes.hanyadik = r;
			kerdes.osszesen = 10;   // ----------   parancssori paraméter ???
		mq_send(msg_queue_id, (char*)&kerdes, sizeof(struct Kerdes), 3);
		mq_close(msg_queue_id);
		
		mq_unlink("kerdes");
		
		waitpid(metro, &status, 0);
		
		pause(); //waits till a signal arrive 
		
		
		printf("szulo -> BMS\n");
		
		exit(0);
		
	} else {
//---------   child -> metro   ---------
		
		// -------------------- MsgQueue --------------
			struct Kerdes a;

			mq_receive(msg_queue_id, &a, sizeof(struct Kerdes), NULL);
			mq_close(msg_queue_id);
			
			printf("%d. szerelveny vagyok.\n%d van meg utanam.\n", a.hanyadik, a.osszesen - 1);
		
			int r = rand() % 100;
			if(r < 10) {
				printf("Sikeresen teljesitettem az uzemet.\n");
				kill(getppid(),SIGRTMIN);
				
				struct Kerdes kerdes;
				kerdes.hanyadik = r;
				kerdes.osszesen = 10;   // ----------   parancssori paraméter ???
				mq_send(msg_queue_id, (char*)&kerdes, sizeof(struct Kerdes), 3);
				mq_close(msg_queue_id);
			}
			else {
				printf("Sikertelen uzem.\n");
				kill(getppid(),SIGRTMIN);
				
				struct Kerdes kerdes;
				
				mq_send(msg_queue_id, (char*)&kerdes, sizeof(struct Kerdes), 3);
				mq_close(msg_queue_id);
					exit(0);
			}
			exit(0);
		
		printf("gyerek -> metro\n");
	}
	
	return 0;
}