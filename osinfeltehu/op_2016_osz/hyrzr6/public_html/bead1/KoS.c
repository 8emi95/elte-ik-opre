
// 1. Operációs Rendszerek Beadandó
// Készítette: Csabai Tamás (HYRZR6)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct Jelentkezo{
	char vezNev[30];
	char kerNev[30];
	char mail[50];
};

struct Rendezveny{
	int azon;
	int jelCount;
	struct Jelentkezo jelentkezok[100];
};

int rendCount = 0;
struct Rendezveny rendezvenyek[10];

int RendInd(const int azon){
	int i;
	int megvan = 0;
	int rendInd = -1;
	for(i = 0;i<rendCount && megvan == 0;i = i+1){
		if(rendezvenyek[i].azon == azon){
			megvan = 1;
			rendInd = i;
		}
	}
	return rendInd;
}

int JelInd(const char mail[50], const int rendInd){
	int i;
	int megvan = 0;
	int jelInd = -1;
	for(i = 0;i<rendezvenyek[rendInd].jelCount && megvan == 0;i = i+1){
		if(strcmp(rendezvenyek[rendInd].jelentkezok[i].mail, mail) == 0){
			megvan = 1;
			jelInd = i;
		}
	}
	return jelInd;
}

int main(int argc, char** argv[]){
	
	
	
	printf("\nUdv a King of Stands latogatoi nyilvantartasaban!\n");
	
	int run = 1;
	do{
		printf("\n-------------------------------------------------\n");
		printf("Kerem valasszon az alabbi menupontok kozul:\n");
		printf("- 1 - Uj rendezveny letrehozasa\n");
		printf("- 2 - Rendezvenyre jelentkezettek listazasa\n");
		printf("- 3 - Rendezveny torlese\n");
		printf("- 4 - Uj vendeg jelentkezese\n");
		printf("- 5 - Vendeg modositasa\n");
		printf("- 6 - Vendeg torlese\n");
		printf("- 0 - KILEPES\n");
		printf("Valasztas: ");
		int valsztas = 1;
		scanf("%d", &valsztas);
		
		switch(valsztas){
			case 0: 
				run = 0;
				break;
				
			case 1:
				printf("Uj rendezveny azonosito szama: ");
				int azon;
				scanf("%d", &azon);
				int rendInd1 = RendInd(azon);
				if(rendInd1 == -1){
					rendInd1 = rendCount;
					int i1;
					for(i1 = 0;i1<rendCount;i1 = i1 + 1)
						if(rendezvenyek[i1].azon == -2)
							rendInd1 = i1;
					rendezvenyek[rendInd1].azon = azon;
					rendezvenyek[rendInd1].jelCount = 0;
					rendCount = rendCount + 1;
					printf("%d azonositoju rendezveny felveve!\n", (rendezvenyek[rendInd1].azon));
				}else
					printf("Mar van %d azonositoju rendezveny!\n", azon);
				break;
				
			case 2:
				printf("Rendezveny azonosito szama: ");
				int azon2;
				scanf("%d", &azon2);
				int rendInd2 = RendInd(azon2);
				if(rendInd2 > -1){
					if(rendezvenyek[rendInd2].jelCount == 0)
						printf("Meg nem jelentkeztek a %d azonositoju rendezvenyre!\n", azon2);
					else{
						int i2;
						for(i2 = 0;i2<rendezvenyek[rendInd2].jelCount;i2 = i2 + 1){
							printf("%d. Jelentkezo: ", i2+1);
							printf("%s ", rendezvenyek[rendInd2].jelentkezok[i2].vezNev);
							printf("%s ", rendezvenyek[rendInd2].jelentkezok[i2].kerNev);
							printf("%s\n", rendezvenyek[rendInd2].jelentkezok[i2].mail);
						}
					}
				}else
					printf("Meg nincs %d azonositoju rendezveny!\n", azon2);
				break;
				
			case 3:
				printf("Rendezveny azonosito szama: ");
				int azon3;
				scanf("%d", &azon3);
				int rendInd3 = RendInd(azon3);
				if(rendInd3 > -1){
					rendezvenyek[rendInd3].azon = -2;
					rendezvenyek[rendInd3].jelCount = 0;
					printf("A %d azonositoju rendezveny torolve!\n", azon3);
				}else
					printf("Meg nincs %d azonositoju rendezveny!\n", azon3);
				break;
				
			case 4:
				printf("Rendezveny azonosito szama: ");
				int azon4;
				scanf("%d", &azon4);
				int rendInd4 = RendInd(azon4);
				if(rendInd4 > -1){
					printf("Vezeteknev: ");
					scanf("%s", rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].vezNev);
					printf("Keresztnev: ");
					scanf("%s", rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].kerNev);
					int jelInd4 = -1;
					do{
						printf("Email: ");
						char mail4[50];
						scanf("%s", mail4);
						jelInd4 = JelInd(mail4, rendInd4);
						if(jelInd4 == -1)
							strcpy(rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].mail, mail4);
						else
							printf("Ezzel az email cimmel mar regisztraltak!\n");
					}while(jelInd4 != -1);
					printf("Sikeres jelentkezes a %d rendezvenyre!\n",rendezvenyek[rendInd4].azon);
					printf("%d. jelentkezo adatai:\n", (rendezvenyek[rendInd4].jelCount)+1);
					printf("Vezeteknev: %s, ",rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].vezNev);
					printf("Keresztnev: %s, ",rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].kerNev);
					printf("Email: %s\n",rendezvenyek[rendInd4].jelentkezok[rendezvenyek[rendInd4].jelCount].mail);
					rendezvenyek[rendInd4].jelCount = rendezvenyek[rendInd4].jelCount + 1;
				}else
					printf("Meg nincs %d azonositoju rendezveny!\n", azon4);
				break;
				
			case 5:
				printf("Jelentkezo email cime: ");
				char mail5[50];
				scanf("%s", mail5);
				int jelInd5 = -1;
				int rendInd5 = -1;
				int i5;
				for(i5 = 0;i5 < rendCount && jelInd5 == -1;i5 = i5 + 1){
					jelInd5 = JelInd(mail5, i5);
					rendInd5 = i5;
				}
				if(jelInd5 != -1){
					printf("Vezeteknev: ");
					scanf("%s", rendezvenyek[rendInd5].jelentkezok[jelInd5].vezNev);
					printf("Keresztnev: ");
					scanf("%s", rendezvenyek[rendInd5].jelentkezok[jelInd5].kerNev);
					int jelInd52 = -1;
					do{
						printf("Email: ");
						char mail5[50];
						scanf("%s", mail5);
						jelInd52 = JelInd(mail5, rendInd5);
						if(jelInd52 == -1)
							strcpy(rendezvenyek[rendInd5].jelentkezok[jelInd5].mail, mail5);
						else
							printf("Ezzel az email cimmel mar regisztraltak!\n");
					}while(jelInd52 != -1);
					printf("Sikeres adatmodositas! Uj adatok:\n",rendezvenyek[rendInd5].jelentkezok[jelInd5].mail);
					printf("Vezeteknev: %s, ",rendezvenyek[rendInd5].jelentkezok[jelInd5].vezNev);
					printf("Keresztnev: %s, ",rendezvenyek[rendInd5].jelentkezok[jelInd5].kerNev);
					printf("Email: %s\n",rendezvenyek[rendInd5].jelentkezok[jelInd5].mail);
				}else
					printf("Ezzel az email cimmel meg nem regisztraltak!\n");
				break;
				
			case 6:
				printf("Jelentkezo email cime: ");
				char mail6[50];
				scanf("%s", mail6);
				int jelInd6 = -1;
				int rendInd6 = -1;
				int i6;
				for(i6 = 0;i6 < rendCount && jelInd6 == -1;i6 = i6 + 1){
					jelInd6 = JelInd(mail6, i6);
					rendInd6 = i6;
				}
				if(jelInd6 != -1){
					printf("%s email cimu jelentkezo torolve!\n", rendezvenyek[rendInd6].jelentkezok[jelInd6].mail);
					strcpy(rendezvenyek[rendInd6].jelentkezok[jelInd6].vezNev, rendezvenyek[rendInd6].jelentkezok[rendezvenyek[rendInd6].jelCount-1].vezNev);
					strcpy(rendezvenyek[rendInd6].jelentkezok[jelInd6].kerNev, rendezvenyek[rendInd6].jelentkezok[rendezvenyek[rendInd6].jelCount-1].kerNev);
					strcpy(rendezvenyek[rendInd6].jelentkezok[jelInd6].mail, rendezvenyek[rendInd6].jelentkezok[rendezvenyek[rendInd6].jelCount-1].mail);
					rendezvenyek[rendInd6].jelCount = rendezvenyek[rendInd6].jelCount - 1;
				}else
					printf("Ezzel az email cimmel meg nem regisztraltak!\n");
				break;
				
			default:
				printf("Nem megfelelo valasztas!\n");
		}
	
	}while(run == 1);
	
	printf("\nViszlat!\n\n");
	return 0;
}