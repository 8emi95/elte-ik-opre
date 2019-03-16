#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//struktura pelda
struct Auto {
	char tipus[100];
	int meret;
};

struct Autok {
	struct Auto egyAuto[1024];
	int darab;
};
//-------------------------

//signal handler
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}
//-------------------------

int main(int argc,char ** argv){
	//argumentumbol beolvasas
	printf("Beolvasott argumentudom száma: %i\n",argc);
	int i;
	for (i=0;i<argc;i++){
		printf("%i. argument is %s\n",i,argv[i]);
	}
	//-------------------------
	
	//signal kezeléshez handler
	signal(SIGUSR1,handler);
	//-------------------------
	
	//fajl kezeles
	FILE *fajlom;
	fajlom = fopen("pelda.txt","a+");  //"r" ha cska olvasni akarok belőle
	if(fajlom == NULL){
		printf("Nem talalni a pelda.txt-t!\n");
		return;
	}else{	
		char sor[512];
		int sorban_levo_szam;
		char sorban_levo_nev[512];
		while(fgets(sor,100,fajlom) != NULL){
			if(strcmp(sor,"") != 0){
				sscanf(sor,"%d %s",&sorban_levo_szam, sorban_levo_nev);
				//printf("%s",sor);
			}
		}
	}
	fclose(fajlom);
	//-------------------------
	
	//random
	srand(time(NULL));
	int r=rand()%100; 
	printf("Random number %i\n",r);
	//-------------------------
	
	//nevesitetlen csovezetekek letrehozasa
	int gyerekIrSzuloOlvas[2];
	int szuloIrGyerekOlvas[2];
	
	//gyerekfolyamat létrehozása
	pid_t  child=fork(); 
	if (child>0){   //szulo folyamat
		close(gyerekIrSzuloOlvas[1]);    //lezarjuk a szuloben az iro veget
		close(szuloIrGyerekOlvas[0]);    //lezarjuk a szuloben az olvaso veget
		
		write(szuloIrGyerekOlvas[1], "Hajra Fradi!",13);
		fflush(NULL);
		
		char sz[100]; 
		read(gyerekIrSzuloOlvas[0],sz,sizeof(sz));
		
		int status;
		waitpid(child,&status,0);        //varunk a gyerek vegere
		close(gyerekIrSzuloOlvas[0]);    //lezarjuk maradekot
		close(szuloIrGyerekOlvas[1]);
	}
	else {    //gyerek folyamat
		close(gyerekIrSzuloOlvas[0]);    //lezarjuk a gyerekben az olvaso veget
		close(szuloIrGyerekOlvas[1]);    //lezarjuk a gyerekben az iro veget
		
		char sz[100]; 
		read(szuloIrGyerekOlvas[0],sz,sizeof(sz));
		
		write(gyerekIrSzuloOlvas[1], "Hajra pelda!",13);
		fflush(NULL);
		
		close(gyerekIrSzuloOlvas[1]);    //lezarjuk maradekot
		close(szuloIrGyerekOlvas[0]);
	}
	//-------------------------
	
	
	
	return 0;
}

// fajlmasolas
void fajlmasolas(const char *honnan, const char *hova){
	FILE *hon;
	FILE *hov;
	
	hon = fopen(honnan,"r");
	hov = fopen(hova,"w");
	
	if(hon == NULL){
		printf("Nem sikerult megnyitni a segito fajlt!\n");
		return;
	}else if (hov == NULL){
		printf("Nem sikerult megnyitni a masolando fajlt!\n");
		return;
	}
	
	char sor[512];
	while(fgets(sor,sizeof(sor),hon) != NULL)
	{
		fputs(sor,hov);
	}
	
	fclose(hon);
	fclose(hov);
	
	remove("segitofile.txt");
}
//-------------------------