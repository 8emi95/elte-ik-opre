#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void menu();

void jelentkezes();
void vendegek_listazasa();
void vendeg_modositas();
void vendeg_torles();

void uj_rendezveny();
void rendezveny_listazas();
void rendezveny_torles();

void fajlmasolas();
int rendezveny_azon_ellenorzes();
int rendezveny_nev_ellenorzes();

int main(int argc,char *argv[]){
	menu();
	
	return 0;
}

void menu(){
	
	char menu[512];
	while (1 == 1){
		
		printf("\n\n VALASSZ A MENUBOL:\n");
		printf(" J\t - Vendeg jelentkezese\n");
		printf(" VL\t - Vendegek listazasa\n");
		printf(" VM\t - Egy vendeg adatainak modositasa\n");
		printf(" VT\t - Egy vendeg adatainak torlese\n");
		printf(" UR\t - Uj rendezveny inditasa\n");		
		printf(" RL\t - Rendezvenyek listazasa\n");
		printf(" RT\t - Rendezveny osszes adatanak torlese\n");
		printf(" CTRL+C\t - KILEPES\n");
		
		
		scanf("%s",menu);
		
		if( strcmp(menu,"J") == 0 )
		{
			jelentkezes();
		}
		else if( strcmp(menu,"VL") == 0 )
		{
			vendegek_listazasa();
		}
		else if( strcmp(menu,"VM") == 0 )
		{
			vendeg_modositas();
		}
		else if( strcmp(menu,"VT") == 0 )
		{
			vendeg_torles();
		}
		else if( strcmp(menu,"UR") == 0 )
		{
			uj_rendezveny();
		}
		else if( strcmp(menu,"RL") == 0 )
		{
			rendezveny_listazas();
		}
		else if( strcmp(menu,"RT") == 0 )
		{
			rendezveny_torles();
		}
		else 
		{
			printf("\n Nincs ilyen menupont! Probald ujra!");
		}
		
	}
	
}

void rendezveny_listazas(){	//rendezveny listazasa
	printf("\n ---RENDEZVENY LISTAZASA---\n");
	
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","r");
	
	if(rendezveny == NULL){
		printf("Nem talalni a rendezvenyeket tarolo fajlt!\n");
		return;
	}else{	
		printf("\n Rendezvenyek:\n");
		
		char sor[512];
		while(fgets(sor,100,rendezveny) != NULL){
			if(strcmp(sor,"") != 0){
				printf("%s",sor);
				//ures sort nem irok ki
			}
		};
	}
	
	fclose(rendezveny);
	
}
void vendegek_listazasa(){	//vendegek listazasa
	printf("\n ---VENDEGEK LISTAZASA---\n");
	
	FILE *vendegek;
	vendegek = fopen("vendegek.txt","r");
	
	if(vendegek == NULL){
		printf("Nem talalni a vendegeket tarolo fajlt!\n");
		return;
	}else{	
		printf("\n Vendegek:\n");
		
		char sor[512];
		while(fgets(sor,100,vendegek) != NULL){
			if(strcmp(sor,"") != 0){
				printf("%s",sor);
				//ures sort nem irok ki
			}
		};
	}
	
	fclose(vendegek);
	
}


void jelentkezes(){
	printf("\n ---VENDEG JELENTKEZESE---\n");
	
	FILE *vendegek;
	vendegek = fopen("vendegek.txt","r");
	
	if(vendegek == NULL){
		printf("Nem talalni a vendegeket tarolo fajlt!\n");
		return;
	}else{
	
		char nev[512];
		char mail[512];
		int rendezvenyazon;
		
		printf("Add meg a vendeg adatait:\n");
		
		printf("Vendeg neve: ");
		scanf("%s",&nev);
		
		printf("Vendeg email cime: ");
		scanf("%s",&mail);
		
		while (1 == 1){
			printf("\nAdd meg a rendezveny azonositojat, vagy irj nullat a rendezvenyek listazasahoz: ");
			scanf("%d",&rendezvenyazon);
			if (rendezvenyazon == 0){
				rendezveny_listazas();
			}
			else if( rendezveny_azon_ellenorzes(rendezvenyazon) ){
				break;
			}
			else{
				printf("\nNincs olyan rendezveny aminek ez az azonositoja! Probald ujra!\n");
			}
		}
		char sor[512];
		int  sor_sorszam;
		char sor_nev[512];
		char sor_mail[512];
		int  sor_rendezvenyazon;
		int  aktualis_sorszam = 1;
		
		FILE *segitofile;
		segitofile = fopen("segitofile.txt","w");
		
		if(segitofile == NULL){
			printf("Nem sikerult letrehozni a segito fajlt!\n");
			return;
		}else{
			int beirtamazujat = 0;
			while(fgets(sor,100,vendegek) != NULL)
			{
				sscanf(sor,"%d %s %s %d",&sor_sorszam, sor_nev, sor_mail, &sor_rendezvenyazon);
				
				
				if ( (strcmp(sor_nev,nev) == 0) && (strcmp(sor_mail,mail) == 0) && (sor_rendezvenyazon == rendezvenyazon)){
					printf("\nMar van ilyen nevu es email cimu vendeg ugyanezzel a rendezvennyel.\n");
					fputs(sor,segitofile);
					aktualis_sorszam += 1;
					beirtamazujat = 1;
				}
				else if ( (strcmp(sor_nev,nev) == 0) && (strcmp(sor_mail,mail) == 0)){
					printf("\nMar van ilyen nevu es email cimu vendeg!\n");
					printf("Szeretne ha a rendezvenyazonositojat az elobb megadottra atirnank?\n Igen (i) Nem (n)\n");
					char valaszto[512];
					scanf("%s",valaszto);
					
					if (strcmp(valaszto,"i") == 0){
						fprintf(segitofile,"%d %s %s %d\n",aktualis_sorszam, &sor_nev, &sor_mail, rendezvenyazon);
						aktualis_sorszam += 1;
						beirtamazujat = 1;
					}else{
						fprintf(segitofile,"%d %s %s %d\n",aktualis_sorszam, &sor_nev, &sor_mail, sor_rendezvenyazon);
						aktualis_sorszam += 1;
						beirtamazujat = 1;
					}
					
				}else{
					fputs(sor,segitofile);
					aktualis_sorszam += 1;
				}
			}
			if (beirtamazujat == 0){
				fprintf(segitofile,"%d %s %s %d \n",aktualis_sorszam, &nev, &mail, rendezvenyazon);
				
				printf("Az uj vendeg bekerult az adatbazisba.\n A sorszama: %d", aktualis_sorszam);
			}
		}
		
		fclose(segitofile);	
	}

	fclose(vendegek);
	
	fajlmasolas("segitofile.txt","vendegek.txt");
}
int rendezveny_azon_ellenorzes(int be_rend_szam){
	
	char sor_rendezveny_nev[512];
	int  sor_rendezvenyazon;
	
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","r");
	
	if(rendezveny == NULL){
		printf("Nem sikerult megnyitni a rendezveny fajlt!\n");
		return;
	}else{
		char sor[512];
		while(fgets(sor,100,rendezveny) != NULL){
			sscanf(sor,"%d %s",&sor_rendezvenyazon, sor_rendezveny_nev);
			if(sor_rendezvenyazon == be_rend_szam){
				return 1;
			}
		}
	}
	fclose(rendezveny);
	return 0;
}
int rendezveny_nev_ellenorzes(const char* be_rend_nev){
	
	char sor_rendezveny_nev[512];
	int  sor_rendezvenyazon;
	
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","r");
	
	if(rendezveny == NULL){
		printf("Nem sikerult megnyitni a rendezveny fajlt!\n");
		return;
	}else{
		char sor[512];
		while(fgets(sor,100,rendezveny) != NULL){
			
			sscanf(sor,"%d %s",&sor_rendezvenyazon, sor_rendezveny_nev);
			
			if(strcmp(sor_rendezveny_nev,be_rend_nev) == 0){
				return 1;
			}
		}
	}
	fclose(rendezveny);
	return 0;
}
void uj_rendezveny(){
	printf("\n ---UJ RENDEZVENY HOZAADASA---\n");
	
	printf("Add meg az uj rendezveny adatait:\n");
	
	
	char rend_nev[512];
	printf("Rendezveny neve: ");
	scanf("%s",&rend_nev);
	
	if(rendezveny_nev_ellenorzes(rend_nev)){
		printf("Mar van ilyen nevu rendezveny!\n");
	}else{
		int rend_azonosito;
		while (1 == 1){
			rend_azonosito = rand() % 100 + 1;
			if(!rendezveny_azon_ellenorzes(rend_azonosito)){
				break;
			}
		}
		FILE *rendezveny;
		rendezveny = fopen("rendezveny.txt","a+");
		
		if(rendezveny == NULL){
			printf("Nem talalni a rendezvenyeket tarolo fajlt!\n");
			return;
		}else{
			fprintf(rendezveny,"\n%d %s",rend_azonosito,&rend_nev);
		}
		
		fclose(rendezveny);
	}
		
}

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


void rendezveny_torles(){
	
	FILE *vendegek;
	vendegek = fopen("vendegek.txt","r");
	
	FILE *rendezveny;
	rendezveny = fopen("rendezveny.txt","r");
	
	FILE *segitofile;
	segitofile = fopen("segitofile.txt","w");
	
	
	int  sor_rendezvenyazon;
	char sor_rendezveny_nev[512];
	
	while (1 == 1){
		int rend_azon;
		
		printf("Add meg a torolni kivant rendezveny azonositojat.\n");
		scanf("%d",&rend_azon);
		
		int vanilyen = 0;
		char sor[512];
		while(fgets(sor,100,rendezveny) != NULL){
			sscanf(sor,"%d %s",&sor_rendezvenyazon, sor_rendezveny_nev);
			printf("%d egyenlp %d\n", sor_rendezvenyazon, rend_azon);
			if(sor_rendezvenyazon == rend_azon){
				vanilyen = 1;
			}else{
				fputs(sor,segitofile);
			}
		}
		printf("eddig eljutott 1.\n");
		if(vanilyen == 0){
			printf("Nincs ilyen azonositoju rendezveny\n");
			fclose(segitofile);	
			fclose(rendezveny);	
			break;
		}else{
			fclose(segitofile);	
			fclose(rendezveny);	
			fajlmasolas("segitofile.txt","rendezveny.txt");
			
			FILE *segitofile;
			segitofile = fopen("segitofile.txt","w");
	
			int  sor_sorszam;
			char sor_nev[512];
			char sor_mail[512];
			int  sor_rendezvenyazon2;
	
			while(fgets(sor,100,vendegek) != NULL)
			{
				sscanf(sor,"%d %s %s %d",&sor_sorszam, sor_nev, sor_mail, &sor_rendezvenyazon2);
				if(sor_rendezvenyazon2 != rend_azon){
					fputs(sor,segitofile);
				}
			}
			fclose(segitofile);	
			fclose(vendegek);	
			fajlmasolas("segitofile.txt","vendegek.txt");
			break;
		}
	}
}

void vendeg_modositas(){
	
	int sorszam;
	
	int  sor_sorszam;
	char sor_nev[512];
	char sor_mail[512];
	int  sor_rendezvenyazon;
	while (1 == 1){
		printf("Add meg a vendeg sorszamat, vagy irjon nullat a listazashoz:\n");
		scanf("%d",&sorszam);
		
		if(sorszam == 0){
			vendegek_listazasa();
		}else{
			FILE *vendegek;
			vendegek = fopen("vendegek.txt","r");
			
			char sor[512];
			if(vendegek == NULL){
				printf("Nem sikerult elerni a vendegek fajlt!\n");
				return;
			}else{
				FILE *segitofile;
				segitofile = fopen("segitofile.txt","w");
				
				if(segitofile == NULL){
					printf("Nem sikerult letrehozni a segito fajlt!\n");
					return;
				}else{
					int talaltam = 0;
					while(fgets(sor,100,vendegek) != NULL){
						sscanf(sor,"%d %s %s %d",&sor_sorszam, sor_nev, sor_mail, &sor_rendezvenyazon);
						if (sor_sorszam == sorszam){
							talaltam = 1;
							printf("A jelenlegi adatok:\n");
							printf("%s",sor);
							
							char nev[512];
							char mail[512];
							int rendezvenyazon;
							
							printf("Add meg az uj adatokat!\n");
							
							printf("Vendeg neve: ");
							scanf("%s",&nev);
							
							printf("Vendeg email cime: ");
							scanf("%s",&mail);
							while (1 == 1){
								printf("\nAdd meg a rendezveny azonositojat, vagy irj nullat a rendezvenyek listazasahoz: ");
								scanf("%d",&rendezvenyazon);
								if (rendezvenyazon == 0){
									rendezveny_listazas();
								}
								else if( rendezveny_azon_ellenorzes(rendezvenyazon) ){
									break;
								}
								else{
									printf("\nNincs olyan rendezveny aminek ez az azonositoja! Probald ujra!\n");
								}
							}
							fprintf(segitofile,"%d %s %s %d\n",sor_sorszam, &nev, &mail, rendezvenyazon);
						}else{
							fputs(sor,segitofile);
						}
					}
					if(talaltam == 0){
						printf("\nNincs ilyen sorszamu vendeg!");
					}else{
						printf("\nA modositas kesz!");
					}
				}
				fclose(segitofile);	
			}
			fclose(vendegek);	
			fajlmasolas("segitofile.txt","vendegek.txt");
			break;
			
		}
	}
}
void vendeg_torles(){
	int sorszam;
	
	int  sor_sorszam;
	char sor_nev[512];
	char sor_mail[512];
	int  sor_rendezvenyazon;
	while (1 == 1){
		printf("Add meg a vendeg sorszamat, vagy irjon nullat a listazashoz:\n");
		scanf("%d",&sorszam);
		
		if(sorszam == 0){
			vendegek_listazasa();
		}else{
			FILE *vendegek;
			vendegek = fopen("vendegek.txt","r");
			
			char sor[512];
			if(vendegek == NULL){
				printf("Nem sikerult elerni a vendegek fajlt!\n");
				return;
			}else{
				FILE *segitofile;
				segitofile = fopen("segitofile.txt","w");
				
				if(segitofile == NULL){
					printf("Nem sikerult letrehozni a segito fajlt!\n");
					return;
				}else{
					
					int  aktualis_sorszam = 1;
					int talaltam = 0;
					while(fgets(sor,100,vendegek) != NULL){
						sscanf(sor,"%d %s %s %d",&sor_sorszam, sor_nev, sor_mail, &sor_rendezvenyazon);
						if (sor_sorszam == sorszam){
							talaltam = 1;
						}else{
							fprintf(segitofile,"%d %s %s %d\n",aktualis_sorszam, &sor_nev, &sor_mail, sor_rendezvenyazon);
							aktualis_sorszam += 1;
						}
						
					}
					if(talaltam == 0){
						printf("\nNincs ilyen sorszamu vendeg!");
					}else{
						printf("\nA torles kesz!");
					}
				}
				fclose(segitofile);	
			}
			fclose(vendegek);	
			fajlmasolas("segitofile.txt","vendegek.txt");
			break;
			
		}
	}
}