#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>

//-lrt

int main (int argc, char* argv[])
{
	pid_t child; 
	int uzenetsor;
	char* mqname = "/noname";
	struct mq_attr attr;
	
	mqd_t mq1;
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = 50; 

	mq_unlink(mqname); 
	mq1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
	if ( mq1 < 0 )
	{ 
		printf("mq_open error: %d \n", errno); 
		return 1; 
	} 
	
	char buffer[100];
	
	child = fork(); 
	if ( child > 0 )
	{
		printf("Szulo indul!\n");
		char uzenet[50]="Random Szoveg";
		mq_send(mq1, uzenet, 50, 5 ); 
		mq_send(mq1, "Hajra Debrecen!", 50, 1);
		printf("Szulo ezkuldte az uzenetet!\n");
		wait(NULL); 
		mq_receive(mq1, buffer, 50, NULL);
		printf("A szulo fogadta a masodik uzenetet: %s\n", buffer);
		mq_close(mq1);
		mq_unlink(mqname);
		printf("Szulo vege!\n"); 
	}
	else
	{
		printf("Gyerek indul!\n");
		int db=mq_receive(mq1, buffer, 50,NULL);
		printf("A gyerek fogadta az uzenetet: %s\n",buffer);
		mq_receive(mq1, buffer, 50, NULL);
		printf("A gyerek fogadta a masodik uzenetet: %s\n", buffer);
		mq_send(mq1, "A gyerek valasza!", 50, 1);
		mq_close(mq1);
		printf("Gyerek vege!\n");
		return 0;  
	}
	return 0; 
} 
