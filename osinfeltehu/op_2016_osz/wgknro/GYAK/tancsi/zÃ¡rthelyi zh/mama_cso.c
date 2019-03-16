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

void uzenet_olvas()
{
		printf("\nMama vagyok, üzenet érkezett,olvasok a csobol!\n");
		for(i=0;i<500;i++) uzenet[i]=0;
		read(olvas,uzenet,sizeof(uzenet));
		printf("A papa uzenete: %s \n",uzenet);
		if (!strcmp(uzenet,"vege"))
		{	
		printf("Mama kiszall!\n");
		vege=1;
		//kill(getppid(),SIGUSR1);
		kill(getppid(),9);
		}
}
void uzenet_ir()
{
	if (!vege)
	{
	printf("Mit uzenunk a papanak:"); 
	//scanf("%[^\n]",uzenet); // %[^\n]	
	//getchar();
	//printf("Az elkuildott uzenet: %s \n",uzenet);
	scanf("%s",uzenet);
	//fgets(uzenet,100,stdin);
	//printf("%s hossza %d \n",uzenet,strlen(uzenet));
	//gets(uzenet);
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

    int id_ir=mkfifo("ir.ftc", S_IRUSR|S_IWUSR );
    int id_olvas=mkfifo("olvas.ftc", S_IRUSR|S_IWUSR );
    //printf("id_ir= %i\n",id_ir); 
    // Ha letezik mar ir.ftc vagy olvas.ftc cso, akkor id_ir vagy id_olvas -1 lesz!
    // Igy az elso csevego program letrehozza a ket csovet, majd a masodik
    // már nem hozza létre mert már van! MIndegy az inditasi sorrend!!
    // S_IWGRP, S_IROTH (other jog)
    signal(SIGUSR1,szignal);
	olvas=open("ir.ftc",O_RDONLY); // papa ir az ir.ftc-be, ezert mama olvas
	ir=open("olvas.ftc",O_WRONLY);
    	struct pollfd poll_fds[2];
	poll_fds[0].fd=olvas;
	poll_fds[0].events=POLLIN;
	printf("Mama indul!\n");
	pid=fork();
	if (pid>0)
	{
	while (!vege)
		uzenet_ir();
	printf("Mama vege!\n");
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
