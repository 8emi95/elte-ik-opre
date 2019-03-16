#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void beolvas();
void handler1(int n){
	printf("Jelzek az elso gyereknek");
}

void handler2(int n){
	printf("Jelzek a masodik gyereknek");
}

int main(int argc, char *argv[]){
	/*char kar[1];
	printf("Adjon meg egy karaktert (t/m) : ");
	scanf("%s",kar);
	printf("Kapott karakter: %s\n",kar);	
*/
	//beolvas();
	
	pid_t gyerek1,gyerek2;
	int gyerek1pid, gyerek2pid;
	signal(SIGUSR1,handler1);
	signal(SIGUSR2,handler2);

	
	
	gyerek1 = fork();
	
	if (gyerek1 > 0) 
	{
		printf("\nI am a parent with PID %d and my parent ID is %d\n",getpid(),getppid());
		gyerek2 = fork();
		
		kill(gyerek1pid,SIGUSR1);
		kill(gyerek2pid,SIGUSR2);
		
		printf("Elkuldtem a ket signalt a gyerekeknek.\n");
		if(gyerek2 == 0) 
		{	
			printf("\nI am a child B with PID %d and my parent ID is %d\n",getpid(),getppid());
			gyerek2pid = getpid();
			pause();
		}
		else {
			sleep(1);
		}	
	}else 
	{
		printf("\nI am a child A with PID %d and my parent ID is %d\n",getpid(),getppid());
		gyerek1pid = getpid();
		pause();
	
	}
	
	return 0;

}

void beolvas(){
    char kar[1];
	printf("Adjon meg egy karaktert (t/m) : ");
	scanf("%s",kar);
	printf("\nKapott karakter: %s\n ",kar);
	//return kar;
}