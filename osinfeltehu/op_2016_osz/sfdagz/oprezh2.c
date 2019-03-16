#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>

struct Message {
    int current;
    int max;
};

struct Message2 {
    char* cause;
};

int succ = -1;

void handler1(int signumber){
    succ = 1;
}

void handler2(int signumber){
    succ = 0;
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
	perror("Nincs megadva parameter");
	exit(1);
    }
    signal(SIGUSR1,handler1);
    signal(SIGUSR2,handler2);
    srand(time(NULL));
    int status;

    struct mq_attr attr;

    attr.mq_maxmsg=5; //MAXMSGS
    attr.mq_msgsize=50; //MSGSIZE

    char* mqname="/sor1"; // Message queue neve
    mq_unlink(mqname); // Törli a message queue-t ha már létezik
    char* mqname2="/sor2";
    mq_unlink(mqname2);
    int mq = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr); // Létrehozza a message queue-t
    int mq2 = mq_open(mqname2, O_CREAT|O_RDWR, 0600, &attr);
    if ( mq < 0 ) {
        printf("Hiba a message queue letrehozasa kozben!\n");
        exit(-1);
    }
	
    if ( mq2 < 0 ) {
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
        int linenum;
	
        linenum = rand() % atoi(argv[1]) + 1;
        struct Message uzenet = {linenum, atoi(argv[1])};
	struct Message2 uzenet2;

        mq_send(mq, (const char*) &uzenet, 50, 5); // Elküldi az üzenetet. Az utolsó paraméter a priortiást jelzi

        printf("Szulo: uzenet elkuldve!\n");

        waitpid(child, &status, 0);
	if (succ == 0)
	{
	    mq_receive(mq2, (char*) &uzenet2, 50, NULL);
	    printf("Szulo: A szerelvenyt bevontattak. %s\n", uzenet2.cause);
	}
	else if (succ == 1)
	{
	    printf("Szulo: A szerelveny beert.\n");
	}
	
        mq_close(mq); // Message queue-t bezárja
        mq_unlink(mqname); // Törli a message queue-t
     } else  {
        struct Message uzenet;
	struct Message2 uzenet2;
        mq_receive(mq, (char*) &uzenet, 50, NULL); // Megkapja az üzenetet. A nagyobb prioritású üzenetet fogja elsőnek olvasni
	srand(time(NULL));
	int success = rand() % 100 + 1;
        int left = uzenet.max - uzenet.current;
        printf("Gyerek: En vagyok a(z) %d. szerelveny.\n",uzenet.current);
	printf("Gyerek: Utanam meg %d szerelveny következik.\n", left);
	if (success > 10)
	{
	   printf("Gyerek: Sikeresen teljesitettem az uzemet!\n");
 	   kill(getppid(),SIGUSR1);
	}
	else
	{
	   printf("Gyerek: Nem sikerult teljesitem az uzemet!\n");
           kill(getppid(),SIGUSR2);
	   srand(1);
	   int cause = rand() % 100 + 1;
	   if (cause > 50)
	   {
		uzenet2.cause = "A bevontatas oka: elcsuszas egy bananhejon.";
	   }
	   else
	   {
		uzenet2.cause = "A bevontatas oka: lefagyas a zord telben.";
	   }
	   mq_send(mq2, (const char*) &uzenet2, 50, 5);
	}
        
	mq_close(mq);
        exit(0);
    }




    return 0;
}
