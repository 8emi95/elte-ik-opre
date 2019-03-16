#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>


void handler(int signumber){}

int main(int argc, char* argv[])
{
  signal(SIGTERM, handler);
  char* maxNum = argv[1];
  int max = maxNum[0] - '0';
  printf("%i\n", max);
  srand(time(NULL));
  pid_t child1=fork();
  char* mqname="/s8h1db";
  struct mq_attr attr;
  mqd_t mq1;
  attr.mq_maxmsg=10; //MAXMSGS
  attr.mq_msgsize=50; //MSGSIZE
  mq_unlink(mqname); // torolni ha letezik
  mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
  if ( mq1 < 0 ) { 
	printf("badmqueue: %s",strerror(errno)); 
	return;
  }  
  if(child1 < 0) {
	perror("The fork calling was not succesful\n");
  }
  else if (child1 > 0) { // This means that we are in the parent;
	char buffer[50];
	int k = rand()%max + 1;
	printf("%i\n", k);
	mq_send(mq1, (char *)&k, sizeof(int), 1);
	pause();
	printf("Sent data\n");
	mq_receive(mq1, buffer, strlen(buffer), 1);
	printf("%A\n", buffer);
	mq_close(mq1);
	mq_unlink(mqname);
  }
  else { // This means that we are in the child;
	int azon;
	printf("%i\n", azon);
	mq_receive(mq1, (char *)&azon, sizeof(azon), 0);
	int t = rand()%10;
	char uzenet[50] = "";
	if (t < 1) {
	  strcpy(uzenet, "Elcsuszott a szerelveny egy bananhejon");
	}
	else if (t < 2) {
      strcpy(uzenet, "Lefagyott a szerelveny a zord telben");
	}
	int hatra = max - azon;
	printf("Ez a %i. szerelveny uzenete.\n", azon);
	printf("Hatra van meg: %i.\n", hatra);
	if (t < 2) {
	  printf("Nem ert celba.\n");
	}
	else {
	  printf("Celba ert.\n");
	}
	mq_send(mq1, uzenet, strlen(uzenet), 1);
	return 0;
  }
  return 0;
}
