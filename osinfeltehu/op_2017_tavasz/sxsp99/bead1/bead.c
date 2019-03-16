#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

void menu();
void kerdesListazas();
void veglegesKerdesListazas();
void ujKerdesSor();
void ujKerdes();
void KerdesModositas();
void kerdesTorles();
void kerdesVeglegesites();

typedef int bool;
enum { false, true };
FILE *kerdesSor = NULL;
char fileName[MAX_SIZE] = "bla";
int szam = 0;
struct kerdes {
	int sorszam;
	char kerdes[MAX_SIZE];
	char valasz[4][MAX_SIZE];
	struct tm letrehozasIdeje;
	bool veglegesitve;
};

int main(int argc, char *argv[]){
	
	menu();
	
	return 0;
}

void menu(){
	char valasz;
	do{
		system("clear"); 
		printf("1: Kerdesek listazasa\n");
		printf("2: Vegleges kerdesek listazasa\n");
		printf("3: Uj kerdessor megadasa\n");
		printf("4: Uj kerdes letrehozasa\n");
		printf("5: Kerdes modositasa\n");
		printf("6: Kerdes torlese\n");
		printf("7: Kerdes veglegesites\n");
		printf("8: Kilepes\n");
		valasz = getchar();
		if( valasz == '1'){
			kerdesListazas();
		}else if(valasz == '2'){
			veglegesKerdesListazas();
		}else if( valasz == '3'){
			ujKerdesSor();
		}else if( valasz == '4'){
			ujKerdes();
		}else if( valasz == '5'){
			KerdesModositas();
		}else if( valasz == '6'){
			kerdesTorles();
		}else if( valasz == '7'){
			kerdesVeglegesites();
		}
	}while(valasz != '8');
}

void kerdesListazas(){
	system("clear"); 
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
			for(i = 0; i < 4; i++){
				if(strlen(kerdesek->valasz[i]) == 0){
					break;
				}
				printf("\t%d. %s\n",i+1 ,kerdesek->valasz[i]);
			}
			printf("Letrehozas ideje: %d-%d-%d %d:%d:%d\n", kerdesek->letrehozasIdeje.tm_year + 1900, kerdesek->letrehozasIdeje.tm_mon + 1, kerdesek->letrehozasIdeje.tm_mday, kerdesek->letrehozasIdeje.tm_hour, kerdesek->letrehozasIdeje.tm_min, kerdesek->letrehozasIdeje.tm_sec);
			printf("Veglegesitve: %d\n", kerdesek->veglegesitve);
			printf("\n");
		}
		fclose(kerdesSor);
		printf("Nyomjon meg egy gombot a folytatáshoz!\n");
	}
	getchar();
	getchar();
}
void veglegesKerdesListazas(){
	system("clear"); 
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->veglegesitve == true){
				printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
			for(i = 0; i < 4; i++){
				if(strlen(kerdesek->valasz[i]) == 0){
					break;
				}
				printf("\t%d. %s\n",i+1 ,kerdesek->valasz[i]);
			}
			printf("Letrehozas ideje: %d-%d-%d %d:%d:%d\n", kerdesek->letrehozasIdeje.tm_year + 1900, kerdesek->letrehozasIdeje.tm_mon + 1, kerdesek->letrehozasIdeje.tm_mday, kerdesek->letrehozasIdeje.tm_hour, kerdesek->letrehozasIdeje.tm_min, kerdesek->letrehozasIdeje.tm_sec);
			printf("\n");
			}
		}
		fclose(kerdesSor);
		printf("Nyomjon meg egy gombot a folytatáshoz!\n");
	}
	getchar();
	getchar();
}

void ujKerdesSor(){
	system("clear");
	printf("Kerlek add meg a kerdessor nevet: ");
	scanf(" %s",fileName);
	kerdesSor = fopen(fileName, "ab");
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		printf("A(z) %s kerdessor letrehozva! Nyomjon meg egy gombot a folytatáshoz!\n", fileName);
		fclose(kerdesSor);
	}
	getchar();
	getchar();
}

void ujKerdes(){
	system("clear");
	
	szam=1;
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			szam++;
		}
		fclose(kerdesSor);
	}
	char valasz;
	struct kerdes ujKerdes;
	kerdesSor = fopen(fileName, "ab");
	ujKerdes.sorszam = szam;
	printf("Kérlek add meg a kerdest:\n");
	scanf(" %s",ujKerdes.kerdes);
	int i;
	for(i = 0; i < 4; i++){
		printf("%d. Valasz: ", i+1);
		scanf(" %s",ujKerdes.valasz[i]);
		if(i > 1 && strlen(ujKerdes.valasz[i]) == 0){
				break;
		}
	}
	time_t t = time(NULL);
	ujKerdes.letrehozasIdeje = *localtime(&t);
	ujKerdes.veglegesitve = false;
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		fwrite(&ujKerdes, sizeof(struct kerdes), 1, kerdesSor);
		fclose(kerdesSor);
	}
	getchar();
	getchar();
}

void KerdesModositas(){
	system("clear"); 
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
			printf("Letrehozas ideje: %d-%d-%d %d:%d:%d\n", kerdesek->letrehozasIdeje.tm_year + 1900, kerdesek->letrehozasIdeje.tm_mon + 1, kerdesek->letrehozasIdeje.tm_mday, kerdesek->letrehozasIdeje.tm_hour, kerdesek->letrehozasIdeje.tm_min, kerdesek->letrehozasIdeje.tm_sec);
			printf("Veglegesitve: %d\n", kerdesek->veglegesitve);
			printf("\n");
		}
		fclose(kerdesSor);
	}
	
	char kerdesSzam[MAX_SIZE];
	printf("Melyik kerdest szeretned modositani?\n");
	scanf(" %s", kerdesSzam);
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		char valasz[MAX_SIZE];
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == atoi(kerdesSzam)){
				printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
				printf("Kerlek add meg az uj erteket: ");
				scanf(" %s", valasz);
				strcpy(kerdesek->kerdes, valasz);
				for(i = 0; i < 4; i++){
					printf("\t%d. %s\n",i+1 ,kerdesek->valasz[i]);
					printf("Kerlek add meg az uj erteket: ");
					scanf(" %s", valasz);
					strcpy(kerdesek->valasz[i], valasz);
				}
			}
			fwrite(kerdesek, sizeof(struct kerdes), 1, temp);
		}
		fclose(kerdesSor);
		fclose(temp);
	}
	
	
	kerdesSor = fopen(fileName, "wb");
	temp = fopen("temp", "rb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		while(fread(kerdesek, sizeof(struct kerdes), 1, temp) == 1){
			fwrite(kerdesek, sizeof(struct kerdes), 1, kerdesSor);
		}
		fclose(kerdesSor);
		fclose(temp);
	}
}

void kerdesTorles(){
	system("clear"); 
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
			printf("Letrehozas ideje: %d-%d-%d %d:%d:%d\n", kerdesek->letrehozasIdeje.tm_year + 1900, kerdesek->letrehozasIdeje.tm_mon + 1, kerdesek->letrehozasIdeje.tm_mday, kerdesek->letrehozasIdeje.tm_hour, kerdesek->letrehozasIdeje.tm_min, kerdesek->letrehozasIdeje.tm_sec);
			printf("Veglegesitve: %d\n", kerdesek->veglegesitve);
			printf("\n");
		}
		fclose(kerdesSor);
	}
	
	char kerdesSzam[MAX_SIZE];
	printf("Melyik kerdest szeretned torolni?\n");
	scanf(" %s", kerdesSzam);
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == atoi(kerdesSzam)){
				continue;
			}
			fwrite(kerdesek, sizeof(struct kerdes), 1, temp);
		}
		fclose(kerdesSor);
		fclose(temp);
	}
	
	
	kerdesSor = fopen(fileName, "wb");
	temp = fopen("temp", "rb");
	szam = 1;
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		while(fread(kerdesek, sizeof(struct kerdes), 1, temp) == 1){
			kerdesek->sorszam = szam;
			fwrite(kerdesek, sizeof(struct kerdes), 1, kerdesSor);
			szam++;
		}
		fclose(kerdesSor);
		fclose(temp);
	}
}



void kerdesVeglegesites(){
	system("clear"); 
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
			printf("Letrehozas ideje: %d-%d-%d %d:%d:%d\n", kerdesek->letrehozasIdeje.tm_year + 1900, kerdesek->letrehozasIdeje.tm_mon + 1, kerdesek->letrehozasIdeje.tm_mday, kerdesek->letrehozasIdeje.tm_hour, kerdesek->letrehozasIdeje.tm_min, kerdesek->letrehozasIdeje.tm_sec);
			printf("Veglegesitve: %d\n", kerdesek->veglegesitve);
			printf("\n");
		}
		fclose(kerdesSor);
	}
	
	char kerdesSzam[MAX_SIZE];
	printf("Melyik kerdest szeretned veglegesiteni?\n");
	scanf(" %s", kerdesSzam);
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == atoi(kerdesSzam)){
				kerdesek->veglegesitve = true;
			}
			fwrite(kerdesek, sizeof(struct kerdes), 1, temp);
		}
		fclose(kerdesSor);
		fclose(temp);
	}
	
	
	kerdesSor = fopen(fileName, "wb");
	temp = fopen("temp", "rb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, temp) == 1){
			fwrite(kerdesek, sizeof(struct kerdes), 1, kerdesSor);
		}
		fclose(kerdesSor);
		fclose(temp);
	}
}