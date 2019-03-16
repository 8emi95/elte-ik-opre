#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include <signal.h>
#include <poll.h>

char uzenet[500];
int vege=0;
int i,ir,olvas,pid;

void uzenet_olvas() // gyerek olvas uzenetet
{
		printf("\nPapa vagyok, üzenet érkezett,olvasok a csobol!\n");
		for(i=0;i<500;i++) uzenet[i]=0;
		read(olvas,uzenet,sizeof(uzenet));
		printf("A mama uzenete: %s \n",uzenet);
		if (!strcmp(uzenet,"vege"))
		{
			vege=1;
			kill(getppid(),9); //SIGUSR1);
		}
}
void uzenet_ir() // szulo ir uzenetet
{
	if (!vege)
	{
	printf("Mit uzenunk a mamanak:"); 
	scanf("%s",uzenet);	
	write(ir,uzenet,strlen(uzenet));
	if (!strcmp(uzenet,"vege"))
		{
		vege=1;
		kill(pid,SIGUSR1);
		}
	}
}
void szignal(int i)
{
	vege=1;
}

int main()
{

    mkfifo("ir.ftc", S_IRUSR|S_IWUSR );
    mkfifo("olvas.ftc", S_IRUSR|S_IWUSR );
    // S_IWGRP, S_IROTH (other jog)
    signal(SIGUSR1,szignal);
	ir=open("ir.ftc",O_WRONLY);
	olvas=open("olvas.ftc",O_RDONLY);
    	struct pollfd poll_fds[2];
	poll_fds[0].fd=olvas;
	poll_fds[0].events=POLLIN;
	printf("Papa indul!\n");
	pid=fork();
	if (pid>0)
	{
	while (!vege)
		uzenet_ir();
	printf("Papa vege!\n");
	//wait();
	}
	else
	{
	while (!vege)
	{
		int result=poll(poll_fds,1,500);
		if (result>0) // var egy jelzesre
		{
			uzenet_olvas();
		}
	}
	}
//	printf("Papa vagyok, vegeztem!\n");
	unlink("ir.ftc");
	unlink("olvas.ftc");
    return 0;
}
