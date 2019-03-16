#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

typedef struct
{
unsigned int szam;
char cim[100];
}mess;


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Nincs megadva a lakcim");
    }
    else
    {
        int db;
	char* mqname="/baba1";
        mqd_t mq1;
	struct mq_attr attr;
	 attr.mq_flags   = 0;
  attr.mq_maxmsg  = 10;
  attr.mq_msgsize = sizeof(mess);
  attr.mq_curmsgs = 0;
	//
	mq_unlink(mqname); // delete if exist

        int pipefd[2]; // unnamed pipe file descriptor array
        pid_t pid;
        char sz[100];  // char array for reading from pipe

        if (pipe(pipefd) == -1)
        {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }
        int i = 0;
            pid = fork();	// creating parent-child processes
            if (pid == -1)
            {
                perror("Fork hiba");
                exit(EXIT_FAILURE);
            }

            if (pid > 0)
            {
                mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
                if ( mq1 < 0 ) {
                printf("mq_open error: %d \n",errno);
                exit(1);
                }
                mess buffer;

                sleep(3);	// sleeping a few seconds, not necessary
                close(pipefd[1]);  //Usually we close the unused write end
                printf("A szulo elkezdi olvasni a csobol az adatokat!\n");
                read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
                printf("A lakcim: %s",sz);
                printf("\n");
                close(pipefd[0]); // finally we close the used read end

                pid_t child2 = fork();
                if (child2 == -1)
                {
                    perror("Fork hiba");
                    exit(EXIT_FAILURE);
                }
		char seged[100];
                if(child2>0)
                {
                    mess t;
                    //szulo
                    //
                    /*int i = 0;
                    for(i=0; i< strlen(argv[1]); ++i)
                    {
                        t.cim[i]=argv[1][i];
                    }
                    t.szam=pid;*/
		
                    strcpy(t.cim,argv[1]);
                    t.szam=pid;
                    db=mq_send(mq1,(char *) &t,sizeof(mess), 5 );  //Parent sends a message.
                    sleep(2);

	  db=mq_receive(mq1,(char*) &t, sizeof(mess),NULL);
printf("A taxi : %s\n",t.cim);
                    int status;
                    while( wait(&status)!=-1) {}
                    mq_close(mq1);
                    mq_unlink(mqname);
                    printf("Szulo vege\n");
                }
                else
                {
                    sleep(1);
                    db=mq_receive(mq1,(char*) &buffer, sizeof(mess),NULL);
                    printf("Lakcim: %s\n",buffer.cim);
                    printf("Az utas szama: %d\n", buffer.szam);
			strcpy(buffer.cim, "Elindultam");
			buffer.szam=0;
			db=mq_send(mq1,(char*) &buffer, sizeof(mess), 5 );
                    mq_close(mq1);
                    printf("%d az auto\n",getpid());
                }

            }
            else
            {
                //gyerek

                close(pipefd[0]); //Usually we close unused read end
                write(pipefd[1], argv[1],sizeof(argv[1]));
                close(pipefd[1]); // Closing write descriptor
            }

        return 0;
    }
}
