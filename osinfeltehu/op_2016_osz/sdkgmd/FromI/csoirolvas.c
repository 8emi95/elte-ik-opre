#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <signal.h>

char uzenet[500];
int vege=0;
int fd;

void szuloszignal(int sig)
{
		printf("Szulo vagyok, olvasok a csobol!\n");
        
		//fd=open("fifo.ftc",O_RDWR);
        //write(fd,"Hajra Fradi!\n",12);
		read(fd,uzenet,sizeof(uzenet));
        //close(fd);
		printf("Szulo vagyok, a gyerek uzenet: %s \n",uzenet);
		
		vege=1;
		//kill(pid,SIGUSR2);
}

void gyerekszignal(int sig)
{
	printf("Gyerek vagyok olvasok a csobol!\n");
	
	fd=open("fifo.ftc",O_RDWR);
	read(fd,uzenet,sizeof(uzenet));
	printf("Gyerek vagyok,ezt olvastam a csobol: %s \n",uzenet);
	//close(fd);
	
	printf("Gyerek vagyok, irom a valaszt!");
    fd=open("fifo.ftc",O_WRONLY);
    write(fd,"Vege",4);
    //close(fd);
	kill(getppid(),SIGUSR1);
	
	vege=1;
}

int main(){
    int pid;

    mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
    // S_IWGRP, S_IROTH (other jog)
    // fifo.ftc fájl 
	signal(SIGUSR1,szuloszignal);
    signal(SIGUSR2,gyerekszignal);

	pid = fork();
    
    if(pid>0)   //szulo 
	{
	//char s[1024];
	fd=open("fifo.ftc",O_RDWR);
//	read(fd,s,sizeof(s));
    write(fd,"Hajra Fradi!\n",12);
//	printf("Ezt olvastam a csobol: %s \n",s);
	
	// torles
	printf("Szulo, csoiras vege!\n");
	//close(fd);
	kill(pid,SIGUSR2);
	/*
	printf("Szulo vagyok, elkuldtem a szignalt!");
	
	fd=open("fifo.ftc",O_RDONLY);
 	read(fd,uzenet,sizeof(uzenet));
    close(fd);
	printf("Szulo vagyok, a gyerek uzenet: %s \n",uzenet);
	*/
	while (!vege);
	//wait();
	printf("Szulo vagyok, vegeztem!\n");
	unlink("fifo.ftc");
    }
	else 
	if(pid==0)	//gyerek 
	{
		/*
		printf("Gyerek vagyok, varom a csoiras utan a szignalt!\n");
        fd=open("fifo.ftc",O_RDONLY);
//        write(fd,"Hajra Fradi!\n",12);
		read(fd,uzenet,sizeof(uzenet));
		printf("Gyerek vagyok,ezt olvastam a csobol: %s \n",uzenet);
        close(fd);

		kill(getppid(),SIGUSR1);
		*/
		while (!vege);	
		printf("Gyerek vagyok, irom a vege valaszt!");
		/*
		fd=open("fifo.ftc",O_WRONLY);
		write(fd,"Vege",4);
    close(fd);
		*/
		printf("Gyerek vagyok, vegeztem!\n");
    }
    
    return 0;
}
