#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(){
    int pid;

    mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
    // S_IWGRP, S_IROTH (other jog)
    // fifo.ftc fájl 
    pid = fork();
    
    if(pid>0)   //szulo 
	{
	char s[1024];
	int fd;		
	fd=open("fifo.ftc",O_RDONLY);
	read(fd,s,sizeof(s));
	printf("Ezt olvastam a csobol: %s \n",s);
	close(fd);
	// torles
	unlink("fifo.ftc");
    }
	else 
	if(pid==0)	//gyerek 
	{
        int fd;
		printf("Gyerek vagyok, irok a csobe!\n");
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,"Hajra Fradi!\n",12);
        close(fd);
		printf("Gyerek vagyok, beírtam, végeztem!\n");
    }
    
    return 0;
}
