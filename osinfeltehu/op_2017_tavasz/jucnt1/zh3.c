#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber);

int countLines(char *filename);
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor,char*buffer) 
{ 
     struct uzenet uz;
	 uz.mtype = 5;
	strcpy(uz.mtext,buffer); 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     printf("%s",uz.mtext);
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 
int main(int argc, char *argv[])
{

    signal(SIGTERM,handler);
	int uzenetsor, status,uz2;
	
    key_t kulcs;
    /* create pipe */
	kulcs = ftok(argv[0],1);
	printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

    int pipefd[2];

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

    /* parent process */
	pid_t child2;
    if (0 != child)    
    {         
		child2=fork();
	   if(child2!=0)
		{
			char buffer[100];
			pause();
			read(pipefd[0],buffer,sizeof(buffer)); // reading max 100 chars
			printf("Szulo olvasta uzenet: %s\n\n", buffer);
			sleep(1);
			kuld( uzenetsor,buffer );  // Parent sends a message.
			kill(child2,SIGTERM);
			snprintf(buffer, 100, "%d", child);
			kuld(uz2,buffer);
			printf("%s",buffer);
			sleep(3);
			kill(child2,SIGTERM);
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          //status = msgctl( uz2, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
		}
		else
		{
			pause();
			fogad( uzenetsor );
			pause();
			fogad( uz2 );
		}
       // kill(getppid(), SIGTERM);

      //waitpid(child, &status, 0); 
    }

    /* child process */
    else
    {
        sleep(2);
		char buf[100];
		strcpy(buf,argv[1]);
        printf("Gyerek indul!\n");
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