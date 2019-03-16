#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/msg.h> 

void handler(int signumber);

int countLines(char *filename);
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int main(int argc, char *argv[])
{

    signal(SIGTERM,handler);
    /* create pipe */
    int pipefd[2];
	int uzenetsor, status; 
    key_t kulcs;
	kulcs = ftok(argv[0],1); 
    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    /* create child */
    pid_t child = fork();
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }
	pid_t child2;
    /* parent process */
    if (0 != child)    
    {         
        child2=fork();
		if(child2!=0)
		{
			char buffer[100];
			pause();
			read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
			printf("Gyerek olvasta uzenet: %s\n\n", buffer);
			sleep(1);
			kill(getppid(), SIGTERM);
			struct uzenet uz;
            uz.mtype=5; 
            uz.mtext="Hajra Fradi!"; 
            //strcpy(uz.mtext,"Hajra Fradi!");
            int status; 
     
            status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 ); 
             /*if ( status < 0 ) 
					perror("msgsnd");
        int status2=msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 );
                if ( status2 < 0 ) 
                    perror("msgsnd");*/

            pid_t wpid;
            while ((wpid = wait(&status)) > 0);
                status = msgctl( uzenetsor, IPC_RMID, NULL ); 
                //status2 = msgctl( uz2, IPC_RMID, NULL ); 
            //printf("%i\n",status2);
            if ( status < 0 ) 
               perror("msgctl");

			//

		}
		else
		{
			struct uzenet uz; 
            int status; 
            status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
            if ( status < 0 ) 
                 perror("msgsnd"); 
            else
                printf( "Az 1. gyerek kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 

		}

      waitpid(child, &status, 0); 
    }

    /* child process */
    else
    {
        sleep(2);
		char buf[100];
		strcpy(buf,argv[1]);
        printf("Szulo indul!\n");
        write(pipefd[1], buf, sizeof(buf));
        sleep(1);
        kill(child, SIGTERM);
    }

    return 0;
}

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}

int countLines(char *filename)
{
    FILE *fp = fopen(filename, "r");

    char c;
    int lines = 1;

    while ((c = fgetc(fp)))
    {
        if (c == EOF)
            break;
        if (c == '\n')
        {
            lines++;
        }
    }
    fclose(fp);

    return lines;
}