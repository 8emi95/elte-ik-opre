#include <stdio.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/wait.h> //waitpid
//call it with some parameters from the command line 

struct Message {
	int a;
	int b;
};


void handler(int signumber){
	printf("Megkapta(signallal): Beert rendben\n");
}

void handler2(int signumber){
	printf("Megkapta(signallal): Nem ert be \n");
}

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;

  int num = atoi(argv[1]);
  printf("%i\n", num);
  
  srand(time(NULL)); //the starting value of random number generation
  int r=rand()%num; 
  printf("Random number %i\n",r);
  
  
  
  
  //////////////////////////////////////////////////////////
  signal(SIGUSR1,handler);
   signal(SIGUSR2,handler2);
   int status;

	struct mq_attr attr;

	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 

    char* mqname="/sor"; // Message queue neve
	mq_unlink(mqname); // Törli a message queue-t ha már létezik

	int mq = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr); // Létrehozza a message queue-t
	
	char uzenet3[40];
    
    if ( mq < 0 ) { 
        printf("Hiba a message queue letrehozasa kozben!\n"); 
        exit(-1); 
    }
	
	int j;
	for( j = 0; j< num; j++){
    int child = fork(); 
    if(child < 0) {
    	printf("Hiba a forkolas kozben!\n");
    	exit(-2);
    }

    if ( child > 0 ) { 
        // Szülő
        struct Message uzenet = {num, r};
	  	mq_send(mq, (const char*) &uzenet, 50, 5); // Elküldi az üzenetet. Az utolsó paraméter a priortiást jelzi
	  	printf("Szulo: uzenet elkuldve!\n");
	  	
			pause();
		
		char azuzenet[40];
		 mq_receive(mq, (char*) &azuzenet, 50, NULL); // Megkapja az üzenetet. A nagyobb prioritású üzenetet fogja elsőnek olvasni
		printf("Megkapta a szulo(messagequen): %s \n", azuzenet );
		
		
	  	waitpid(child, &status, 0);

        mq_close(mq); // Message queue-t bezárja
	  	mq_unlink(mqname); // Törli a message queue-t
		
		
	
		
     } else  {
        struct Message uzenet;
        mq_receive(mq, (char*) &uzenet, 50, NULL); // Megkapja az üzenetet. A nagyobb prioritású üzenetet fogja elsőnek olvasni
        //printf("%d %c %d = %d\n", uzenet.a, uzenet.op, uzenet.b, uzenet.result);
		printf("Az uzenet: maximum szam: %d     en vagy a :%d    hatra van: %d \n" , uzenet.a, uzenet.b, uzenet.a-uzenet.b);
		
		  srand(time(NULL)); //the starting value of random number generation
		int k=rand()%10; 
			printf("Random number(1-nel meghibasodik, egyb. nem) %i\n",k);
			
			int parent = getppid();
				sleep(1);
			int meghibasodott = 0; // 1 ha igen
			
			if(k == 1){
				meghibasodott = 1;
				printf("Meghibasodtam :(   \n");
				char uzenett [40] = " Rossz idojaras miatt";
				strcpy(uzenet3,uzenett);
				
				kill(parent, SIGUSR2);
			}else{
				meghibasodott = 0;
				printf("Nem hibasodtam meg :)) \n");
				char uzenett [40] = " Jo idojaras miatt";
				strcpy(uzenet3,uzenett);
				
				kill(parent, SIGUSR1);
			}
	 
		
		
	printf("%s\n",uzenet3);
	
	
	mq_send(mq, (const char*) &uzenet3, 50, 5); // Elküldi az üzenetet. Az utolsó paraméter a priortiást jelzi
	printf("Gyerek: uzenet elkuldve!\n");
	
	mq_close(mq);
	
	
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
		
		
		
		
		
		
		
		exit(0);
	 }
    }
 
 return 0;
}