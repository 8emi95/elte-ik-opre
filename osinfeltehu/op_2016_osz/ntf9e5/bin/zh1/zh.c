#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


volatile sig_atomic_t interrupt = 0;
volatile sig_atomic_t sender_queue_id = 0;

// signal handler from sigaction_value.c
void handler(int signumber,siginfo_t* info,void* nonused){
    interrupt = 1;
    switch (info->si_code){ 
        case SI_USER: /*for kill*/ break;
        case SI_TIMER: /*for timer*/ break;
        case SI_QUEUE: /*for sigqueue*/
        {
            sender_queue_id = info->si_value.sival_int;
            break;
        }
    default: printf("It was sent by something else \n");
  } 
}

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

/*void init_random(int i)
{
    srand(time(NULL) + i * 1024);
}

int generate_number(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}
*/

int main(int argc, char *argv[])
{
	/*if(argc < 2)
	{
		usage();
		return -1;
	}
	const int MAXCHILD = atoi(argv[1]);
	if(MAXCHILD < 1 || MAXCHILD > 7)
	{
		usage();
		return -1;
	}*/
	
    sigset_t active_sigmask, empty_sigset;
	
	sigemptyset(&active_sigmask); //empty signal set
	sigemptyset(&empty_sigset); //empty signal set

	sigaddset(&active_sigmask, SIGTERM); //SIGTERM is in set
	
    // from sigaction_value.c, for signal additional value
	struct sigaction sigact;
    sigact.sa_sigaction=handler; //instead of sa_handler, we use the 3 parameter version, to get the additional value
    sigemptyset(&sigact.sa_mask); 
    sigact.sa_flags=SA_SIGINFO; //we need to set the siginfo flag to get additional value
    sigaction(SIGTERM,&sigact,NULL); 
	
    sigprocmask(SIG_BLOCK,&active_sigmask,NULL);
    
	int parent_queue = 0;
	key_t parent_queue_key;
	
	parent_queue_key = ftok(argv[0],1); // Important to use unique number.
	parent_queue = msgget(parent_queue_key,0600 | IPC_CREAT);
	
    int child = fork();
    
    if (child < 0)
    {
        perror("fork error");
    }
	else if (child > 0)
    {
        // Parent process
        // 1. feladat
        while (interrupt == 0)
        {
            sigsuspend(&empty_sigset);
        }
        interrupt = 0;
        // get date
        // TODO
        // convert date to string
        // TODO
        // assign date to a variable
//        struct uzenet uz;
  //      uz.mtype = 10;
    //    strcpy(uz.mtext, "kuldok valamit");
const struct uzenet uz = { 3, "Pentek van!" };
const struct uzenet uz2 = { 5, "Szombat van!" };
        int sender = sender_queue_id;
        printf("sender: %i\n", sender);
        // send it back to the child who sent the signal
        int status = msgsnd( sender, &uz, 1024 , 0 ); 
        status = msgsnd( sender, &uz2, 1024 , 0 ); 
        if ( status < 0 ) 
        {
            perror("msgsnd");
        }
		sleep(1);
        // 1. feladat vége
    }
    else
    {
        // Child process
        int child_queue = 0;
        key_t child_queue_key;
        char* nev = "Debrecen";
	
        child_queue_key = ftok(argv[0], 2); // Important to use unique number.
        child_queue = msgget(child_queue_key,0600 | IPC_CREAT);
        printf("cqueue: %i\n", child_queue);
        
        union sigval value = {child_queue};
        sigqueue(getppid(),SIGTERM, value);
        
        struct uzenet uz; 
        int status = msgrcv(child_queue, &uz, 1024, 5, 0 ); 
	//	printf("%i",child_queue);

        // convert uz.mtext to date
        // TODO
        // add 1 to the date
        // TODO
        // print date and name
        // TODO
        printf("date: name: %s\n", nev);
		printf("%i\n", uz.mtype);
		printf("%s\n", uz.mtext);
		//sleep(1);
		//printf("%s", uz);
        // 1. feladat vége
    }
}
