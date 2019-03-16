#ifndef VENDEG_H
#define VENDEG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define LENGTH 10
#define MAXVENDEG 10
#define SORHOSSZ 150

void vendeg_menu();
void vendeg_lista();
void beszur();
void szerkeszt();
void split(struct Vendeg *vendeg, char* str);
void structstr(struct Vendeg *vendeg, char* str);
void keres(char* str[MAXVENDEG], int i);
void modosit(struct Vendeg *vendeg);

void vendeg_menu(){
	char menupont[LENGTH];
	printf("--------------------------------\n");
	printf("Vendegek\n\n");
	printf("j - Jelentkezes\n");
	printf("l - Listazas\n");
	printf("m - Modositas\n");
	//printf("Vendeg torlese: t\n");
	printf("Vissza: x\n");
	
	scanf("%s",menupont);
	
	if(strcmp("j", menupont) == 0){
		beszur();
	}
	else if(strcmp("l", menupont) == 0){
		vendeg_lista();
	}
	else if(strcmp("m", menupont) == 0){
		szerkeszt();
	}
	else if(strcmp("x", menupont) == 0){
		menu();
	}
	
}


void vendeg_lista(){
	FILE *vendeg;
	vendeg = fopen("vendegek.txt", "r");
	
	printf("\n--------------------------------\n");
	
	if(vendeg == NULL){
		printf("Nincs lista\n");
	} else{
		char sor[SORHOSSZ];
		
		while(fgets(sor,SORHOSSZ,vendeg) != NULL){
			printf("%s",sor);
		}
		
		fclose(vendeg);
	}
	vendeg_menu();
	
}



void beszur(){
	FILE *vendeg;
	vendeg = fopen("vendegek.txt", "a+");
	
	char sor[SORHOSSZ];
	char vnev[20];
	char knev[20];
	char email[40];
	char rendezveny_azon[2];
	
	printf("--------------------------------\n");
	printf("Vezetknev: ");
	scanf("%s", vnev);
	printf("Keresztnev: ");
	scanf("%s", knev);
	printf("e-mail cim: ");
	scanf("%s", email);
	printf("Rendezveny azonosito: ");
	scanf("%s", rendezveny_azon);
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,80,"%X",timeinfo);
	
	int i=1;
	while(fgets(sor, SORHOSSZ, vendeg) != NULL){++i;}
	
	fprintf(vendeg,"%s %s %s %s %s\n",vnev,knev,email,rendezveny_azon,buffer);
	
	fclose(vendeg);
	
	printf("%i. vendeg\n",i);

	vendeg_menu();
	
}

struct Vendeg {
		char vnev[10];
		char knev[10];
		char email[25];
		char rend[10];
		char idopont[10];
	};

void szerkeszt(){
	
	char* str[MAXVENDEG];
	char sor[SORHOSSZ];
	int i=0;
	

	FILE *vendeg;
	vendeg = fopen("vendegek.txt", "r+");
	
	if(vendeg != NULL){
		while( (fgets(sor,SORHOSSZ,vendeg) != NULL) && (i < MAXVENDEG) ){
			
			str[i] = (char*) malloc(SORHOSSZ);
			strcpy(str[i],sor);
			printf("sor -> %s", sor);
				
			i++;
		}
		fclose(vendeg);
	}
	
	keres(str, i);
	
	
	vendeg = fopen ("vendegek.txt","w");
	
	int j=0;
	while( (str[j] != NULL) && (j < i) ){
		//printf("str -> %s",str[j]);
		fprintf(vendeg, "%s", str[j]);
		j++;
	}
	
	fclose(vendeg);
	vendeg_menu();
}

void keres(char* str[MAXVENDEG], int i){
	char vnev[10];
	char knev[10];
	struct Vendeg Vendegek;
	
	printf("--------------------------------\n");
	printf("Adja meg a keresett vendeg nevet\n");
	printf("Vezeteknev: ");
	scanf("%s",vnev);
	printf("Keresztnev: ");
	scanf("%s",knev);
	
	
	char tmp[SORHOSSZ];
	int j=0;
	while(j<i){
		strcpy(tmp,str[j]);
		split(&Vendegek, tmp);
		if( !strcmp(vnev, Vendegek.vnev) && !strcmp(knev, Vendegek.knev) ){
			
			printf("--------------------------------\n");
			printf("Vezeteknev: %s\n",Vendegek.vnev);
			printf("Keresztnev: %s\n",Vendegek.knev);
			printf("e-mail cim: %s\n",Vendegek.email);
			printf("Rendezveny azonosito: %s\n",Vendegek.rend);
			
			
			printf("--------------------------------\n");
			printf("m - Modositas\n");
			printf("t - Torles\n");
			char ch[4];
			scanf("%s", ch);
			
			if( !strcmp(ch, "m") ){
				modosit(&Vendegek);
				structstr(&Vendegek, str[j]);
			}
			else if( !strcmp(ch, "t") ){
				printf("Biztosan torol? (i/n)");
				scanf("%s", ch);
				if( !strcmp(ch, "i") ){
					strcpy(str[j],"");
				}
			}
			
		}
		j++;
		
	}
	
	
}

void modosit(struct Vendeg *vendeg){
	
	printf("--------------------------------\n");
	printf("Mit szeretne javitani?\n");
	printf("v - Vezeteknev\n");
	printf("k - Keresztnev\n");
	printf("e - e-mail cim\n");
	printf("r - Rendezveny azonosito\n");
		
	char ch[4];
	scanf("%s", ch);
	printf("--------------------------------\n");
			
	if( !strcmp(ch, "v") ){
		printf("Vezeteknev: ");
		scanf("%s", vendeg->vnev);
	}
	else if( !strcmp(ch, "k") ){
		printf("Keresztnev: ");
		scanf("%s", vendeg->knev);
	}
	else if( !strcmp(ch, "e") ){
		printf("e-mail cim: ");
		scanf("%s", vendeg->email);}
	else if( !strcmp(ch, "r") ){
		printf("Rendezveny azonosito: ");
		scanf("%s", vendeg->rend);
	}
	
}

void structstr(struct Vendeg *vendeg, char* str){
	strcpy(str, vendeg->vnev);
	/*
	strcpy(str, "\n");
	strcat(str, vendeg->vnev);
	*/
	strcat(str, " ");
	strcat(str, vendeg->knev);
	strcat(str, " ");
	strcat(str, vendeg->email);
	strcat(str, " ");
	strcat(str, vendeg->rend);
	strcat(str, " ");
	strcat(str, vendeg->idopont);
	//strcat(str, "\n");
}

void split(struct Vendeg *vendeg, char* str){
	
	strcpy( vendeg->vnev, strtok (str," ") );
	strcpy( vendeg->knev, strtok (NULL," ") );
	strcpy( vendeg->email, strtok (NULL," ") );
	strcpy( vendeg->rend, strtok (NULL," ") );
	strcpy( vendeg->idopont, strtok (NULL," ") );
	/*
	printf("vnev: %s\n",vendeg->vnev);
	printf("knev: %s\n",vendeg->knev);
	printf("email: %s\n",vendeg->email);
	printf("rend: %s\n",vendeg->rend);
	printf("idopont: %s\n",vendeg->idopont);
	*/
}


#endif /* VENDEG_H */