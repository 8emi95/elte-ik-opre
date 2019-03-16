#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>


struct Kerdes{
  char kerd[256];
  char va[256];
  char v2[256];
  char v3[256];
  char v4[256];
  time_t ido;
};


void listaz(struct Kerdes kerdesek[], int db){
  int i=0;
  for(i=0;i<db;i++){
    printf("\n");
    printf("%i. kerdes:\n",i+1);
    printf("kerdes letrehozva: %s", asctime(localtime(&kerdesek[i].ido)));
    printf("\n");
    printf("kerdes: ");
    printf(kerdesek[i].kerd);
    
    printf("1.valasz: ");
    printf(kerdesek[i].va);
    printf("\n");
    printf("2.valasz: ");
    printf(kerdesek[i].v2);
    printf("\n");
    printf("3.valasz: ");
    printf(kerdesek[i].v3);
    printf("\n");
    printf("4.valasz: ");
    printf(kerdesek[i].v4);
    printf("\n");
    }
}

void torol(struct Kerdes *kerdesek, int db, int n){
	int i;
	for(i=n;i<db-1;i++){
		strcpy(kerdesek[i].kerd, kerdesek[i+1].kerd);
		strcpy(kerdesek[i].va, kerdesek[i+1].va);
		strcpy(kerdesek[i].v2, kerdesek[i+1].v2);
		strcpy(kerdesek[i].v3, kerdesek[i+1].v3);
		strcpy(kerdesek[i].v4, kerdesek[i+1].v4);
		kerdesek[i].ido= kerdesek[i+1].ido;
		
	}
	strcpy(kerdesek[i].kerd, kerdesek[255].kerd);
	strcpy(kerdesek[i].va, kerdesek[255].va);
	strcpy(kerdesek[i].v2, kerdesek[255].v2);
	strcpy(kerdesek[i].v3, kerdesek[255].v3);
	strcpy(kerdesek[i].v4, kerdesek[255].v4);
	kerdesek[i].ido= kerdesek[255].ido;

}

void modositas(struct Kerdes *mkerd){
	char mker[256];
	char m2ker[256];
	printf("\nkerdes: \n");
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->kerd, mker);
	printf("1. valasz: \n");
	//scanf(" %s", &mker);
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->va, mker); 
	printf("2. valasz: \n");
	//scanf(" %s", &m2ker);
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v2, mker);
	printf("3. valasz: \n");
	//scanf(" %s", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v3, mker); 
	printf("4. valasz: \n");
	//scanf(" %s", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v4, mker);
	mkerd->ido=time(0);


}

void menu(){
  printf("1:Kerdes felvetel\n");
  printf("2:Kerdes modositasa\n");
  printf("3:Kerdes torlese\n");
  printf("4:Kerdesek listazasa\n");
  printf("5:Kedoiv veglegesitese\n");
  printf("6:Veglegesitett kerdoiv megtekintese\n");
  printf("7:Kilepes es mentes\n");
}

int main(){
	struct Kerdes kerdesek[256];
	int db=0;
	struct Kerdes vkerdesek[256];
	
	int handle = open("./data.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	int vhandle = open("./vegleges.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if (handle < 0)
 	{
       		printf("Fajl hiba.");
   	}
	
	if (vhandle < 0)
 	{
       		printf("Fajl hiba.");
   	}

	read(handle, &kerdesek, sizeof(kerdesek));
	read(vhandle, &vkerdesek, sizeof(vkerdesek));
	for(;;){
		if(strlen(kerdesek[db].kerd)==0)
			break;
		else
			db++;
	}



	int s=0;
	for(;;){
		if(strlen(vkerdesek[s].kerd)==0)
			break;
		else
			s++;
	}

	char c;
	while(1==1){
	menu();
	scanf(" %c", &c);
	switch(c){
		case '1' :
			modositas(&kerdesek[db++]);
			break;
		case '2':
			listaz(kerdesek, db);
			scanf(" %c", &c);
			if(c<db||c>0){
				modositas(&kerdesek[c-'0'-1]);
			}else{
				printf("%i db kerdes van\n",db);
			}
			break;
		case '3' :
			listaz(kerdesek, db);
			printf("Torlendo kerdes szama: \n");
			scanf(" %c", &c);
			if(c<db||c>0){
				torol(&kerdesek,db,c-'0'-1);
			}else{
				printf("%i db kerdes van\n",db);
			}
			db--;
			break;
		case '4' :
			listaz(kerdesek, db);
			break;
		case '5' :
			s=0;
			while(1 == 1){
				listaz(kerdesek, db);
				printf("Hanyas szamu legyen az %i.kerdes(x a lista befejezesehez): \n",s+1);
				scanf(" %c", &c);

				if(c<db||c>0||c=='x'){
					if(c=='x')
						break;
					else{
						
						strcpy(vkerdesek[s].kerd, kerdesek[c-'0'-1].kerd);
						strcpy(vkerdesek[s].va, kerdesek[c-'0'-1].va);
						strcpy(vkerdesek[s].v2, kerdesek[c-'0'-1].v2);
						strcpy(vkerdesek[s].v3, kerdesek[c-'0'-1].v3);
						strcpy(vkerdesek[s].v4, kerdesek[c-'0'-1].v4);
						vkerdesek[s].ido= kerdesek[c-'0'].ido;
						s++;
						
					}
				}else{
					printf("%i db kerdes van\n",db);
				}
			}
			break;
		case '6' :
			listaz(vkerdesek, s);
			break;
			
		case '7':
			
	
	
			remove("./data.dat");
			remove("./vegleges.dat");

			int handle = open("./data.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
 			if (handle < 0)
 			{
       				printf("Fajl hiba.");
   			}
   			write(handle, &kerdesek, sizeof(kerdesek));



			int vhandle = open("./vegleges.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
   			if (vhandle < 0)
   			{
       				printf("Fajl hiba.");
   			}
   			write(vhandle, &vkerdesek, sizeof(vkerdesek));
			exit(1);
		default :
			printf("1 es 7 kozotti szamot var a program(%c)",c);

	}
	}
	

}
