#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // for errno, the number of last error
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#define LIMIT 100
#define MAXVALASZ 4

void handler(int signumber){}

struct Kerdes{
	char kerdes[128];
	char valasz[MAXVALASZ][128];
	int vdb;
	time_t ido;
	bool kitoltve; //felhasználjuk a kérdések kiválasztására
};

int kdb = 0;
struct Kerdes kerdesek[LIMIT];

void listaz(){
	printf("\n");
	int i = 0;
	for(i; i<kdb;i++){
		printf("A(z) %d. kerdes(veglegesitett: %s): %s \t hozzaadas datuma: %s",i+1,kerdesek[i].kitoltve ? "igen" : "nem",kerdesek[i].kerdes,ctime(&kerdesek[i].ido));
		int j=0;
		for(j; j<kerdesek[i].vdb;j++){
			printf("\t -%s\n",kerdesek[i].valasz[j]);
		}
	}
}
void fileBe(){
    FILE *fp;
    fp=fopen("tmp.txt", "r");
    
    if (fp == NULL){
        printf("Betoltes sikertelen!!");
        return;
}
    
    fread(&kdb, sizeof(int), 1, fp);
    int i;
    for (i = 0; i < kdb; i++) {
        fread(&kerdesek[i], sizeof(struct Kerdes), 1, fp);        
    }
    fclose(fp);
};

int main(int argc,char ** argv)
{
	if(argc != 2){
		printf("\nHiányos adatok\n");
		return 1;
	}
	fileBe();
	int i = 0;
	for(i; i<kdb;i++){
		kerdesek[i].kitoltve = false;
	}
	signal(SIGRTMAX,handler);
	printf("\nMLSZ konzultáció\n");
	int fd1;
	unlink("pipe");
    int fid1=mkfifo("pipe", S_IRUSR|S_IWUSR );
	if (fid1==-1){
        printf("Error number: %i",errno); 
        exit(EXIT_FAILURE);
    }	
	srand(time(NULL)); //the starting value of random number generation
	pid_t pid = fork();
	
	if(pid>0){ //parent
		pause(); //waits till a signal arrive 
		int db = 1;		
		do
		{
			int r = rand()%db;
			if(!kerdesek[r].kitoltve){
				kerdesek[r].kitoltve = true;
				++db;
			}
		}while(db!=3);
		fd1=open("pipe",O_WRONLY);
		write(fd1,kerdesek,sizeof(kerdesek));
		close(fd1);
		fd1=open("pipe",O_RDONLY);
		struct Kerdes valaszolt[LIMIT];
		read(fd1,valaszolt,sizeof(valaszolt));
		close(fd1);		
		
		i = 0;
		for(i; i<kdb;i++){
			if(valaszolt[i].kitoltve){
				printf("A(z) %d. kerdesre a kitoltok valaszai: %s \n    Valaszok:\n",i+1,kerdesek[i].kerdes);
				int j=0;
				for(j; j<valaszolt[i].vdb;j++){
					printf("\t -%-15s \t\t%s db valaszado\n",kerdesek[i].valasz[j],valaszolt[i].valasz[j]);
				}
			}
		}
		
	}
	else
	{
		kill(getppid(),SIGRTMAX); 
		fd1=open("pipe",O_RDONLY);
		struct Kerdes kiValasz[LIMIT];
		read(fd1,kiValasz,sizeof(kiValasz));
		close(fd1);		
		int edb = 10 + (rand() % 11);
		int i = 0;
		for(i; i<kdb;i++){
			int j =0;
			for(j; j<kiValasz[i].vdb;j++){
				memset(&kiValasz[i].valasz[j], 0, sizeof(kiValasz[i].valasz[j]));
				sprintf(kiValasz[i].valasz[j],"%d",0);
			}
		}
		for(i; i < kdb;i++){
			if(kiValasz[i].kitoltve){
				int k =0;
				for(k; k< edb;k++){
					int val = rand() % kiValasz[i].vdb;
					int db = atoi(kiValasz[i].valasz[val]);
					sprintf(kiValasz[i].valasz[val],"%d",++db);
				}
			}
		}
		fd1=open("pipe",O_WRONLY);
		write(fd1,kiValasz,sizeof(kiValasz));
		close(fd1);		
	}
    return 0;
}
