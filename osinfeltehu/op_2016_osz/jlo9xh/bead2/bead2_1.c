#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <string.h>
#include <time.h> 

#define MAX 150
#define SORHOSSZ 20

void parent(char* ch);
void handler(int signalnumber)
{
	printf("A partner megerkezett\n");
}

void handler2 (int signumber, siginfo_t* info, void* nonused)
{
	int buffer;
	buffer = (int)(info->si_value.sival_ptr);
	printf("A rendezveny sikere 10/%i\n", buffer);
}

int main(int argc, char* argv[])
{
	
	if (argc != 2)
	{
		perror("Kerem a rendezveny helyszinet parameterben!");
		exit(1);
	}
	char ch[MAX];
	stpcpy(ch, argv[1]);
	parent(ch);
	return 0;
}

void parent(char* ch)
{
	
	unlink("fifopout.ftc");
	unlink("fifopin.ftc");
	int fd;
	int fido=mkfifo("fifopout.ftc", S_IRUSR|S_IWUSR );
	int fidi=mkfifo("fifopin.ftc", S_IRUSR|S_IWUSR );
	
	pid_t chpid;
	char sz[MAX]; 

	if (fido == -1 || fidi == -1)
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1,handler);
	
	struct sigaction sigact; 
	sigact.sa_sigaction = handler2;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &sigact, NULL);
	
	chpid = fork();
	if (chpid == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	
	if (chpid == 0)
	{	
		printf("\nPartner indul\n");
		sleep(3);
		
		fd=open("fifopout.ftc",O_RDONLY);
		read(fd,sz,sizeof(sz));
		printf("\nHelyszin: %s\n",sz);
		
		printf("\nUtazas 3 masodperc\n");
		sleep(3);
		printf("\nMegerkezett\n");
		kill(getppid(),SIGUSR1);

		printf("\nResztvevok:\n");
		char* names[SORHOSSZ];
		int i=0;
		
		srand(time(NULL));
		read(fd,sz,sizeof(sz));
		while(strcmp(sz,"x") != 0)
		{
			if( 40 > (rand() % 100) )
			{
				names[i] = (char*) malloc(SORHOSSZ);
				strcpy(names[i], sz);
				i++;
			}
			printf("%s",sz);
			read(fd,sz,sizeof(sz));
		}
		close(fd);
		unlink("fifopout.ftc");
		
		char tmp[SORHOSSZ];
		
		int j=0;
		fd=open("fifopin.ftc",O_WRONLY);
		
		printf("\nMeg 3 masodperc a rendezveny vegeig");
		sleep(3);
		srand(time(NULL));
		union sigval my_sigval;
		my_sigval.sival_ptr = (void*)(rand() % 10);
		sigqueue(getppid(), SIGUSR2, my_sigval);

		while(j<i)
		{
			strcpy(tmp, names[j]);
			write(fd,tmp,SORHOSSZ);
			sleep(1);
			j++;
		}
		write(fd,"x",2);
		close(fd);
		
		printf("\nPartner vege\n");
	} 
	else
	{ 
		printf("\nSzervezo indul\n");
		fd=open("fifopout.ftc",O_WRONLY);
		write(fd,ch,sizeof(ch));

		pause();

		FILE *vendeg;
		vendeg = fopen("vendegek.txt", "r");
		char sor[SORHOSSZ];
		while( fgets(sor,SORHOSSZ,vendeg) != NULL)
		{
			write(fd,sor,sizeof(sor));
			sleep(1);
		}
		write(fd,"x",2);
		fclose(vendeg);
		close(fd);
		
		fd=open("fifopin.ftc",O_RDONLY);
		
		printf("\nSzervezo var a partnerre\n");
		pause();

		printf("\nHianyzok:\n");
		read(fd,sz,sizeof(sz));
		while(  strcmp(sz,"x") != 0  )
		{
			printf("%s",sz);
			read(fd,sz,sizeof(sz));
			
		}
		close(fd);
		unlink("fifopin.ftc");
		
		int status;
		wait(&status);
		printf("\nSzervezo vege\n");
	}
	
	
}