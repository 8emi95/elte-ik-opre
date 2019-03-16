#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>

int main(int argc,char* argv[]){
	
     int uzenetsor, status;
	char* mqname="/baba1";
	char* mqname2="/baba2";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	//
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0666,&attr);
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
	 mq2=mq_open(mqname2, O_CREAT|O_RDWR,0666,&attr);
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
     char buffer[100];	

	pid_t pid = fork();	
	if(pid>0){ //parent
	 wait( NULL );
	} else { //child
		int status2;
		pid_t pid2 = fork();	
		if(pid2>0){ //child Kevin
			srand(time(NULL) + pid2 ); //the starting value of random number generation
			int r=rand()%31;
			r=r+20;
			char str[5];
			sprintf(str,"%i",r);
			mq_send(mq1,str,strlen(str)+2, 4);
			 waitpid(pid2,&status2,0); 
			 int db=mq_receive(mq2,buffer, 54,NULL);
			 printf("Ajaj a betorok ezt uzentek: %s\n",buffer);
			mq_close(mq1);
		mq_unlink(mqname);
		mq_close(mq2);
		mq_unlink(mqname2);
		exit(0); 
		} else { // child Betoro
			int db=mq_receive(mq1,buffer, 54,NULL);
			printf("En a betoro %s-szer fogok hanyattesni. \n",buffer);
			mq_close(mq1);
			mq_send(mq2,"Csak varj mig a kezunk koze kerulsz",50,4);
		}
		
	}
	
	
	return 0;
}