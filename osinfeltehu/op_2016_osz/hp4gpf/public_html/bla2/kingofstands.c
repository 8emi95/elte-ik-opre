#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

struct Jelentkezes {
	char nev[50];
	char mail[50];
	int id;
	int index;
	time_t ido;
};

void UjJelentkezes(){
	int noofstructs = NoOfStructs();
	FILE* f = fopen("jelentkezesek.data", "ab+");
	struct Jelentkezes j;
	printf("Név: ");
	fgets(j.nev, 50, stdin); //Enélkül üresen hagyja a nevet
	fgets(j.nev, 50, stdin);
	printf("E-mail: ");
	fgets(j.mail, 50, stdin);
	printf("Rendezvény azonosítója: ");
	scanf("%d", &j.id);
	j.id=noofstructs+1;//TODO max id + 1
	j.ido=time(0);
	char * str = "%d %s %s %d %s", osszes[i].id, osszes[i].nev, osszes[i].mail, osszes[i].index, c_time;
	fwrite(str, sizeof(struct Jelentkezes), 1, f);
	printf("Jelentkezés sikeres! Te vagy a %d. vendég!\n", j.id);
	fclose(f);
}

void UjRendezveny(){
	printf("Új Rendezvény azonosítója: ");
	int azon;
	scanf("%d", azon);
	
}

void JelBeolvas(struct Jelentkezes osszes[], int n){
	int i;
	for(i=1; i<n; i++){
		char* c_time = ctime(&osszes[i].ido);
		printf("%d %s %s %d %s", osszes[i].id, osszes[i].nev, osszes[i].mail, osszes[i].index, c_time);
	}
}

int VendegEllenorzes(struct Jelentkezes osszes[], int n, int vendeg){
	int i;
	for(i=0; i<n; i++){
		if(osszes[i].index == vendeg){
			return 1;
		}
	}
	return 0;
}

/*
int NoOfStructs(){
	struct stat st;
	stat("jelentkezesek.data", &st);
	int filesize = st.st_size;
	int structsize = sizeof(struct Jelentkezes);
	if(filesize%structsize != 0){
		printf("HIBA: Fájlméret nem osztható a struct méretével!");
		printf("File size: %d\n", filesize);
		printf("Struct size: %d\n", structsize);
		exit(1);
	}
	return filesize/structsize;
}
*/


void JelentkezesModosit(struct Jelentkezes osszes[], int n){
	printf("Jelentkezés ID-je: ");
	int id;
	scanf("%d", &id);
	int ellenorzes = VendegEllenorzes(osszes[], n, id);
	while(ellenorzes ==0){
		printf("Vendég ID nem létezik! Próbálja újra: ");
		scanf("%d", &id);
		int ellenorzes = VendegEllenorzes(osszes[], n, id);
	}
	//TODO: Egy rekord kiírása
	printf("Mit szeretnél módosítani? Név[N] E-mail[E] RendezvényID[R] Mégsem[X]");
	char c=getchar();
		c=toupper(c);
		switch(c){
			case 'N':
				printf("Új név: ");
				char s[50];
				fgets(s, 50, stdin);
				stpcpy()//TODO: this
				break;
			case 'E':
				printf("Új e-mail: ");
				char s[50];
				fgets(s, 50, stdin);
				stpcpy()//TODO: this
				break;
			case 'R':
				
			case 'X':
				b = 1;
				break;
			default:
				break;
		}
}

int main(){
	int b = 0;
	while(b==0){
		FILE* f = fopen("jelentkezesek.data", "rb");
		int n = NoOfStructs();
		struct Jelentkezes osszes[n];
		int i;
		for(i=0; i<n; i++){
			fread(&osszes[i], sizeof(struct Jelentkezes), 1, f);
		}
		fclose(f);
		printf("Üdvözlet a King of Stands nyilvántartásában! Lehetőségek:\nÚj fájl[U] Listázás[L] Kilépés[X] Jelentkezés módosítása[M]\n");
		char c=getchar();
		c=toupper(c);
		switch(c){
			case 'U':
				UjJelentkezes();
				break;
			case 'L':
				JelBeolvas(osszes, n);
				break;
			case 'M':
				
			case 'X':
				b = 1;
				break;
			default:
				break;
		}
		getchar();
	}
	return 0;
}