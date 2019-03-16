#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/wait.h> //waitpid

/*
	-lrt segítségével kell fordítani
*/

struct Message {
	int a;
	char op;
	int b;
	int result;
};

int main (int argc, char* argv[]) { 
    int status;

	struct mq_attr attr;

	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 

    char* mqname="/sor"; // Message queue neve
	mq_unlink(mqname); // Törli a message queue-t ha már létezik

	int mq = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr); // Létrehozza a message queue-t
    
    if ( mq < 0 ) { 
        printf("Hiba a message queue letrehozasa kozben!\n"); 
        exit(-1); 
    }

    int child = fork(); 
    if(child < 0) {
    	printf("Hiba a forkolas kozben!\n");
    	exit(-2);
    }

    if ( child > 0 ) { 
        // Szülő
        struct Message uzenet = {1, '+', 1, 2};
        struct Message uzenet2 = {2, '+', 2, 4};

	  	mq_send(mq, (const char*) &uzenet, 50, 5); // Elküldi az üzenetet. Az utolsó paraméter a priortiást jelzi
	  	mq_send(mq, (const char*) &uzenet2, 50, 6); // Elküldi az üzenetet. Az utolsó paraméter a priortiást jelzi



	  	printf("Szulo: uzenet elkuldve!\n");
	  	
	  	waitpid(child, &status, 0);

        mq_close(mq); // Message queue-t bezárja
	  	mq_unlink(mqname); // Törli a message queue-t
     } else  {
        struct Message uzenet;
        mq_receive(mq, (char*) &uzenet, 50, NULL); // Megkapja az üzenetet. A nagyobb prioritású üzenetet fogja elsőnek olvasni
        printf("%d %c %d = %d\n", uzenet.a, uzenet.op, uzenet.b, uzenet.result);
		mq_close(mq);
		exit(0);
    }
     
    return 0; 
} 
