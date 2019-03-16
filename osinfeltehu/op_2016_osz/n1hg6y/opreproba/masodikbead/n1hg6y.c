
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

struct name
{
	char name[160];
};


void handler(int signumber){
}

int main(int argc,char** argv)
{
	//random beállítása
    srand(time(NULL)); //the starting value of random number generation
	// signal beállítása
	signal(SIGTERM,handler);
	//
	 FILE * f;
	 f=fopen("rendezveny.txt","r");
	 if (f==NULL){perror("File opening error\n"); exit(1);}
	 // beolv
	 
	 char line[160];
	 char place[160];
	 struct name names[20];
	 char num[160];
	 int i;
	 if(!feof(f))
	 {
		 fgets(num,sizeof(num),f);
		 i =(num[0]-'0')*10 +( num[1]-'0');
	 }
	 if(!feof(f))
	 {
		 fgets(place,sizeof(place),f);
	 }
	 int counter=0;
	 while (!feof(f) && counter<i){
	   fgets(names[counter].name,sizeof(names[counter].name),f);
	   printf("%s",names[counter].name);
	   counter++;
	 } 
	 printf("\n");
	 fclose(f);
	 // beolv end
	 
	 //pipeok elkészítése
	 
    int pipeforparentwrite[2]; // unnamed pipe file descriptor array
    int pipeforchildwrite[2]; // unnamed pipe file descriptor array
	 
    if (pipe(pipeforparentwrite) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
    }
    if (pipe(pipeforchildwrite) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
    }
	
	 // forkolás
	 int status;
	 pid_t  child=fork(); //forks make a copy of variables
	 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
	 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
	 {
        close(pipeforchildwrite[1]);  //Usually we close the unused write end
		close(pipeforparentwrite[0]); //Usually we close unused read end
		
		printf("Gyereknek kuldom a helyszint:  --- Szervezo\n");
		printf("%s\n",place);
		
        write(pipeforparentwrite[1], place,sizeof(place));
		sleep(3);
		char feedback[160];
		read(pipeforchildwrite[0],feedback,sizeof(feedback));
		printf("megjott a visszajelzes a gyerektol: --- Szervezo\n");
		printf("%s\n\n",feedback);
		
		
		printf("Kuldom a gyereknek a meghivottakat  --- Szervezo\n");
		int j;
		for(j=0;j<i;j++)
		{
			write(pipeforparentwrite[1], names[j].name,sizeof(names[j].name));
			pause();
		}
		int siker;
		read(pipeforchildwrite[0],&siker,sizeof(siker));
		
		printf("Es vegul a sikeresseg: %d --- Szervezo \n",siker);
        close(pipeforparentwrite[1]); 
		close(pipeforchildwrite[0]); 
		waitpid(child,&status,0); 
	 }
	 else //child process
	 {
        close(pipeforparentwrite[1]);  //Usually we close the unused write end
		close(pipeforchildwrite[0]); //Usually we close unused read end
		char sz[160];
        read(pipeforparentwrite[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Kuldom a visszajelzest a szervezonek,(itt vagyok) --- Gyerek: \n");
		printf("%s\n",sz);
        write(pipeforchildwrite[1], "Megerkeztem",sizeof("Megerkeztem"));
		// meghivottak megszerzese
		sleep(3);
		struct name gotnames[20];
		int j;
		int r; 
		printf("Akik nem erkeztek meg: \n");
		for(j=0;j<i;j++)
		{
			r=rand()%10;
			if(r>0)
			{
				read(pipeforparentwrite[0],gotnames[j].name,sizeof(gotnames[j].name)); 
				//printf("kuldok esemenyt,megerkezett a kovi vendeg: --- Gyerek\n%s\n ",gotnames[j].name);
			}
			else
			{
				read(pipeforparentwrite[0],gotnames[j].name,sizeof(gotnames[j].name)); 
				printf("nem erkezett meg a kovi vendeg: --- Gyerek\n%s\n",gotnames[j].name);
			}
			kill(getppid(),SIGTERM); 
		}
		//
		r=rand()%101;
		write(pipeforchildwrite[1], &r,sizeof(r));
		
        close(pipeforchildwrite[1]); 
		close(pipeforparentwrite[0]); 
	 }
	 
	 return 0;
}