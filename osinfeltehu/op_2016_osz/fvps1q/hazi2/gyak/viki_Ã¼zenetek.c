#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <mqueue.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h> 

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main () { 
     pid_t lanygyerek;
	 pid_t fiugyerek;
	 
	 const char * lanynev="Anna";
	 const char * fiunev="Barna";
	 
	 int cso_esemeny_hely1[2]; 
	 int cso_esemeny_hely2[2];
     char csotartalom1[100]; 
	 char csotartalom2[100]; 
	 
	 signal(SIGTERM,handler);
	 
	 
     int uzenetsor, status; 
     
	 pipe(cso_esemeny_hely1);
	 pipe(cso_esemeny_hely2);
	 
    char* mqname="/baba1";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	
	mq_unlink(mqname); // delete if exists
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
	 
     char buffer[100];	// mq_send, mq_receive uses char array
	 printf("Buffer length: %d\n",strlen(buffer));
	
     
     lanygyerek = fork(); 
     if ( lanygyerek > 0 ) { 
	 
			fiugyerek=fork();
			
			if (fiugyerek>0)
			{
				close(cso_esemeny_hely1[0]);
				write(cso_esemeny_hely1[1],lanynev,strlen(lanynev));
				close(cso_esemeny_hely1[1]);

				close(cso_esemeny_hely2[0]);
				write(cso_esemeny_hely2[1],fiunev,strlen(fiunev));
				close(cso_esemeny_hely2[1]);
				
				printf("\nBeleírt a szülő a csőbe\n");
				
				//kill(lanygyerek,SIGTERM);
			    //kill(fiugyerek,SIGTERM);

				//kuld( uzenetsor );  // Parent sends a message. 
				//wait( NULL ); 
				// After terminating child process, the message queue is deleted. 
				/*status = msgctl( uzenetsor, IPC_RMID, NULL ); 
				if ( status < 0 ) 
					perror("msgctl"); 
				return 0; */
				wait(NULL);
				wait(NULL);

				
			}
			else{
				sleep(3);
				close(cso_esemeny_hely2[1]);
				read(cso_esemeny_hely2[0],csotartalom1,sizeof(csotartalom1));
				close(cso_esemeny_hely2[0]);
					printf("%s\n", csotartalom1);
					
				
				
				//int mq_send(mqd_t mqdes, const char *msg_ptr,size_t msg_len, unsigned int msg_prio);
					 
				
				//sleep(1); // ha ez nincs a fogad be is olvassa!!!!
				mq_send(mq1,csotartalom1,strlen(csotartalom1), 4);
				//mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
					printf("Fiú: név elkuldve!");
				
				kill(lanygyerek,SIGTERM);		
			}
		  
     } else if ( lanygyerek == 0 ) { 
			sleep(4);
			close(cso_esemeny_hely1[1]);
			read(cso_esemeny_hely1[0],csotartalom2,sizeof(csotartalom2));
			close(cso_esemeny_hely1[0]);
					printf("%s\n", csotartalom2);
					
		    pause();
			mq_receive(mq1, buffer, strlen(buffer), 0); 
				printf("Lány: köszi az üzikédet %s", buffer);
				
				
			
		//  return fogad( uzenetsor ); 
	
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
