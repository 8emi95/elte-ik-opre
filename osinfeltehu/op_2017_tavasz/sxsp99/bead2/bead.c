#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 512

void menu();
void kerdesListazas();
void veglegesKerdesListazas();
void ujKerdesSor();
void ujKerdes();
void KerdesModositas();
void kerdesTorles();
void kerdesVeglegesites();
void kerdezoBiztos();

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
		printf("8: Kerdezobiztos\n");
		printf("9: Kilepes\n");
		char line[256];
		if (fgets(line, sizeof(line), stdin)) {
			sscanf(line, "%c", &valasz);	
		}
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
		}else if( valasz == '8'){
			kerdezoBiztos();
		}
	}while(valasz != '9');
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
}

void ujKerdesSor(){
	system("clear");
	printf("Kerlek add meg a kerdessor nevet: ");
	fgets(fileName, sizeof(fileName), stdin);
	int i = 0;
	while(fileName[i] != 0){
		if(fileName[i] == ' ')
			fileName[i] = '_';
		i++;
	}
	kerdesSor = fopen(fileName, "ab");
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		printf("A(z) %s kerdessor letrehozva! Nyomjon meg egy gombot a folytatáshoz!\n", fileName);
		fclose(kerdesSor);
	}
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
	do{
		fgets(ujKerdes.kerdes, sizeof(ujKerdes.kerdes), stdin);
	}while(strlen(ujKerdes.kerdes) == 1);
	int i;
	for(i = 0; i < 4; i++){
		printf("%d. Valasz: ", i+1);
		do{
			fgets(ujKerdes.valasz[i], sizeof(ujKerdes.valasz[i]), stdin);
		}while(strlen(ujKerdes.valasz[i]) == 1 && i < 2);
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
	
	int kerdesSzam;
	printf("Melyik kerdest szeretned modositani?\n");
	char line[256];
	if (fgets(line, sizeof(line), stdin)) {
		sscanf(line, "%d", &kerdesSzam);	
	}
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		char valasz[MAX_SIZE];
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == kerdesSzam){
				printf("%d. %s\n",kerdesek->sorszam, kerdesek->kerdes);
				printf("Kerlek add meg az uj erteket: ");
				fgets(valasz, sizeof(valasz), stdin);
				strcpy(kerdesek->kerdes, valasz);
				for(i = 0; i < 4; i++){
					printf("\t%d. %s\n",i+1 ,kerdesek->valasz[i]);
					printf("Kerlek add meg az uj erteket: ");
					fgets(valasz, sizeof(valasz), stdin);
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
	
	int kerdesSzam;
	char line[256];
	printf("Melyik kerdest szeretned torolni?\n");
	if (fgets(line, sizeof(line), stdin)) {
		sscanf(line, "%d", &kerdesSzam);	
	}
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == kerdesSzam){
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
	
	int kerdesSzam;
	char line[256];
	printf("Melyik kerdest szeretned veglegesiteni?\n");
	if (fgets(line, sizeof(line), stdin)) {
		sscanf(line, "%d", &kerdesSzam);	
	}
	
	kerdesSor = fopen(fileName, "rb");
	FILE *temp = fopen("temp", "wb");
	if(kerdesSor == NULL || temp == NULL){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->sorszam == kerdesSzam){
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

void kerdezoBiztos(){
	system("clear"); 
	char line[255];
	kerdesSor = fopen(fileName, "rb");
	struct kerdes *kerdesek=malloc(sizeof(struct kerdes));
	struct kerdes *veglegesitettkerdesek=malloc(3*sizeof(struct kerdes));
	struct kerdes *olvas=malloc(3*sizeof(struct kerdes));
	int kerdesSzam = 0;
	if(kerdesSor == NULL ){
		printf("Hiba a fajl letrehozasaban! Nyomjon meg egy gombot a folytatáshoz!\n");
	}else{
		int i;
		while(fread(kerdesek, sizeof(struct kerdes), 1, kerdesSor) == 1){
			if(kerdesek->veglegesitve == true){
				strcpy(veglegesitettkerdesek[kerdesSzam].sorszam, kerdesek->sorszam);
				strcpy(veglegesitettkerdesek[kerdesSzam]->kerdes, kerdesek->kerdes;)
				veglegesitettkerdesek[kerdesSzam]->veglegesitve = 1;
			for(i = 0; i < 4; i++){
				if(strlen(kerdesek->valasz[i]) == 0){
					break;
				}
				strcpy(veglegesitettkerdesek[kerdesSzam]->valasz[i], kerdesek->valasz[i]);
			}
			strcpy(veglegesitettkerdesek[kerdesSzam]->letrehozasIdeje, kerdesek->letrehozasIdeje);
			}
			if(kerdesSzam > 2){
					break;
				}
			kerdesSzam++;
		}
		fclose(kerdesSor);
		
		int pipefd[2]; // unnamed pipe file descriptor array
		pid_t pid;
		  // char array for reading from pipe

		if (pipe(pipefd) == -1){
			   perror("Hiba a pipe nyitaskor!");
			   exit(EXIT_FAILURE);
		}
		pid = fork();	// creating parent-child processes
		if (pid == -1){
			   perror("Fork hiba");
			   exit(EXIT_FAILURE);
		}
		if (pid == 0){	// child process
			close(pipefd[0]); //Usually we close unused read end
			write(pipefd[1], "ready",5);
			close(pipefd[1]); // Closing write descriptor 
			wait();
			close(pipefd[1]);
			read(pipefd[0],olvas, 3*sizeof(struct kerdes));
			close(pipefd[0]);	
			printf("%s", olvas[0].kerdes);
		   /*close(pipefd[1]);  //Usually we close the unused write end
		   printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		   read(pipefd[0],olvas,sizeof(struct kerdes)); // reading max 100 chars
		   printf("Gyerek olvasta uzenet: %s",olvas->kerdes);
		   printf("\n");
		   close(pipefd[0]); // finally we close the used read end*/
		   exit(EXIT_SUCCESS);
		}else{    // szulo process 
		    printf("Szulo indul!\n");
			close(pipefd[1]);  //Usually we close the unused write end
			read(pipefd[0],line, 5); // reading max 100 chars
			close(pipefd[0]);
			wait();
			if(strcmp(line, "ready") == 0){
				close(pipefd[0]);
				write(pipefd[1], veglegesitettkerdesek,3*sizeof(struct kerdes));
				close(pipefd[1]);
			}
		   
		   /*close(pipefd[0]); //Usually we close unused read end
		   write(pipefd[1], kerdesek,sizeof(struct kerdes));
		   close(pipefd[1]); // Closing write descriptor 
		   printf("Szulo beirta az adatokat a csobe!\n");
		   //fflush(NULL); 	// flushes all write buffers (not necessary)*/
		   wait();		// waiting for child process (not necessary)
			// try it without wait()
		   printf("Szulo befejezte!");	
		}
	}
	getchar();
}