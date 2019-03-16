#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include <signal.h>

char uzenet[500];
int vege=0;
int i,fd,pid;

void szuloszignal(int sig)
{
		printf("Szulo vagyok, olvasok a csobol!\n");
		for(i=0;i<500;i++) uzenet[i]=0;
		read(fd,uzenet,sizeof(uzenet));
		printf("Szulo vagyok, a gyerek uzenet: %s \n",uzenet);
		if (!strcmp(uzenet,"vege"))
			vege=1;
		else
		{
			printf("Mit uzenunk a gyereknek:"); 
			scanf("%s",uzenet);	
			
			write(fd,uzenet,strlen(uzenet));
			kill(pid,SIGUSR2);
		}
}

void gyerekszignal(int sig)
{
	printf("Gyerek vagyok olvasom a szulo uzenetet a csobol!\n");
	read(fd,uzenet,sizeof(uzenet));
	printf("Gyerek vagyok,ezt olvastam a csobol: %s \n",uzenet);
	
	printf("Gyerek vagyok,mi legyen a valaszt:");
	for(i=0;i<500;i++) uzenet[i]=0;
	scanf("%s",uzenet);
    	write(fd,uzenet,strlen(uzenet));//"Vege",4);
    	
	kill(getppid(),SIGUSR1);
	if (!strcmp(uzenet,"vege"))
	vege=1;
}

int main(){

    mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
    // S_IWGRP, S_IROTH (other jog)
    // fifo.ftc fájl 
	signal(SIGUSR1,szuloszignal);
    	signal(SIGUSR2,gyerekszignal);

	pid = fork();
    
	fd=open("fifo.ftc",O_RDWR);
    if(pid>0)   //szulo 
	{
    	write(fd,"Hajra Fradi!\n",12);
	// torles
	printf("Szulo, csoiras vege!\n");
	//close(fd);
	kill(pid,SIGUSR2);
	/*
	printf("Szulo vagyok, a gyerek uzenet: %s \n",uzenet);
	*/
	while (!vege)
	{
	pause(); // var egy jelzesre
	}
	printf("Szulo vagyok, vegeztem!\n");
	unlink("fifo.ftc");
    }
	else 
	if(pid==0)	//gyerek 
	{
		/*
		kill(getppid(),SIGUSR1);
		*/
		while (!vege)
		{
		pause(); // var egy jelzesre
		}	
		printf("Gyerek vagyok, vegeztem!\n");
    }
    
    return 0;
}
