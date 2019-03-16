       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
       #include <signal.h>
       #include <sys/ipc.h> 
       #include <sys/msg.h> 
       #include <sys/types.h>
       #include <wait.h> 
       #include <fcntl.h>
       #include <sys/stat.h> 
       #include <mqueue.h>




int main(int argc, char *argv[])
{
        char maxszerelvenyszam;
        int szerelvenyszam;
    int uzenetsor, status;
	char* mqname="/baba1";
	struct mq_attr attr;

	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE
    
    mq_unlink(mqname);
    mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
     char buffer[100];
   
     // maxszerelvenyszam;
        if (fork()){
            //szulo
          printf("valami");
        maxszerelvenyszam = argv[1];
        // szerelvenyszam = rand() % maxszerelvenyszam;
              
              
                char uzenet[30] = maxszerelvenyszam;
        
                //uzenet[13]=65; uzenet[14]=65; uzenet[15]=65;
                  int db=mq_send(mq1,uzenet,17, 5 ); 
                   //mq_send(mq1,"Hajra Debrecen!",20, 4);
                  wait( NULL ); 
                     mq_close(mq1);
                     	  mq_unlink(mqname);
            //exit(0);
        }
        else{
            //gyerek
              int db=mq_receive(mq1,buffer, 54,NULL);
               printf("mq_receive : %s, olvasott bájtok hossza: %d \n",buffer, db);
               //db=mq_receive(mq1,buffer, 50, 0);

            //printf("kovetkező uzenet: %s hossza: %d\n", buffer,db);
            mq_close(mq1);
            return 0;
            //printf("Ez is valami");
            //exit(0);
        }

return 0;
}