#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define INFILE "output.bin"

struct kerdes{
char szoveg[5][50];
time_t ido;
};


struct kerdes* kerdesek = 0;
int kerdes_db = 0;
int kerdes_max = 0;
int keszGyerek = 1;
int gyerekSzam;

void kiir(struct kerdes* kerdes){
	printf("%s", kerdes->szoveg[0]); 
	int i = 1;
	for (i =1; i<5; i++){
		printf("%d %s", i, kerdes->szoveg[i]);
	}
}

void hozzaad_kerdes(struct kerdes* kerdes)
{
	printf("hozzaad_kerdes");
	if(kerdes_db == kerdes_max)
	{
		struct kerdes* tmp;
		tmp = (struct kerdes*)malloc(2*kerdes_max*sizeof(struct kerdes));
		int i;
		for(i = 0; i < kerdes_max;++i)
		{
			tmp[i] = kerdesek[i];
		}
		free(kerdesek);
		kerdesek = tmp;
		kerdes_max *= 2;
		printf("hozzaad_kerdes_in");
	}
	kerdesek[kerdes_db] = *kerdes;
	++kerdes_db;
}

void be(){
	printf("BE");
	int f = open(INFILE,O_RDONLY); 
    if (f<0){ perror("Error at opening the file\n");exit(1);}
	struct kerdes tmp;
	int j = 0;
	while (read(f,&tmp,sizeof(struct kerdes))){ 
		hozzaad_kerdes(&tmp);
	}
}


void handler(int signumber)
{
  if (signumber==SIGUSR1)
  {	
		++keszGyerek;
  }
}

void csinaljGyereket(char* csonev){
	kill(getppid(),SIGTERM);
	srand(getpid());
	int pipe = open(csonev,O_RDWR);
	struct kerdes myKerdes[3];
	read(pipe,&myKerdes[0],sizeof(struct kerdes));
	read(pipe,&myKerdes[1],sizeof(struct kerdes));
	read(pipe,&myKerdes[2],sizeof(struct kerdes));
	int j;
	for (j=0; j<3; j++){
		int i=2;
		while (strcmp(myKerdes[j].szoveg[i+1],"") && i < 4){++i;}
		int val = rand()%i;
		write(pipe,&val,sizeof(int));
	}
	close(pipe);
}


int main(){
	signal(SIGUSR1,handler);
	kerdes_max = 1;
	kerdesek=(struct kerdes*)malloc(kerdes_max*sizeof(struct kerdes));
	be();
	srand(time(NULL));
	gyerekSzam = 10+rand()%11;
	pid_t *gyerek;
	gyerek=(pid_t*)malloc(gyerekSzam*sizeof(pid_t));
	
	int *cso;
	cso=(int*)malloc(gyerekSzam*sizeof(int));
	
	int konz[3];
	konz[0]=rand()%kerdes_db;
	konz[1]=rand()%kerdes_db;
	konz[2]=rand()%kerdes_db;
	
	int valasz[20][3] = {0};
	while(konz[0]==konz[1]){
		konz[1]=rand()%kerdes_db;
	}
	
	while(konz[0]==konz[2] || konz[1]==konz[2]){
		konz[2]=rand()%kerdes_db;
	}
	
	int i;
	for(i = 0; i < gyerekSzam; ++i)
	{
		char tmp[100] = "pipe";
	    char buffer[10];
	    sprintf(buffer, "%d", i);
	    strcat(tmp,buffer);
	    unlink(tmp);
	    cso[i] = mkfifo(tmp, 0777 );
	    if ( cso[i] == -1)
	    {
	      printf("Hiba a %s cso nyitasa kozben!\n",tmp);
	      exit(EXIT_FAILURE);
	    }
	    cso[i] = open(tmp, O_RDWR);
	    gyerek[i] = fork();
	    if(gyerek[i]<0) 
	    {
	      printf("Hiba a %d gyerek letrehozasa kozben!\n",i);
	      exit(EXIT_FAILURE);
	    } else if (gyerek[i]==0) 
	      {				
			csinaljGyereket(tmp);
			exit(EXIT_SUCCESS);
	      }
	}
	while (keszGyerek != gyerekSzam) { }
	for(i = 0; i < gyerekSzam; ++i){
		write(cso[i],&kerdesek[konz[0]],sizeof(struct kerdes));
		write(cso[i],&kerdesek[konz[1]],sizeof(struct kerdes));
		write(cso[i],&kerdesek[konz[2]],sizeof(struct kerdes));
	}
	sleep(1);
	for(i = 0; i < gyerekSzam; ++i){
		read(cso[i],&valasz[i][0],sizeof(int));
		read(cso[i],&valasz[i][1],sizeof(int));
		read(cso[i],&valasz[i][2],sizeof(int));
	}
	int j;
	printf("Kerdezo biztosok szama: %d\n",gyerekSzam);
	for (j =0; j<3; j++){
		int ossz[4] = {0};
		for(i = 0; i < gyerekSzam; ++i){
			++ossz[valasz[i][j]];
		}
		printf("%s", kerdesek[konz[j]].szoveg[0]);
		i = 1;
		while (strcmp(kerdesek[konz[j]].szoveg[i],"") && i<5){
			printf("%d %s", i, kerdesek[konz[j]].szoveg[i]);
			printf("%d db\n", ossz[i-1]);
			++i;
		}
	}
	for(i = 0; i < gyerekSzam; ++i)
		close(cso[i]);
	for(i = 0; i < gyerekSzam; ++i)
	{
		char tmp[100] = "pipe";
	    char buffer[10];
	    sprintf(buffer, "%d", i);
	    strcat(tmp,buffer);
	    unlink(tmp);
	}
	free(kerdesek);
	free(gyerek);
	free(cso);
	return 0;
}