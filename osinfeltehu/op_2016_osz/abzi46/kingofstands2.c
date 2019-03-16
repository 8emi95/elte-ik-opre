#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

struct Jelentkezes {
	char nev[50];
	char mail[50];
	int id; //Rendezvény IDje
	int index; //Vendég Indexe 
	time_t ido;
};

void UjJelentkezes(int maxindex, int rendezvenyek[]){
	int noofstructs = NoOfStructs();
	FILE* f = fopen("jelentkezesek.data", "ab+");
	struct Jelentkezes j;
	printf("Név: ");
	fgets(j.nev, 50, stdin); //Enélkül üresen hagyja a nevet
	fgets(j.nev, 50, stdin);
	printf("E-mail: ");
	fgets(j.mail, 50, stdin);
	printf("Rendezvény azonosítója: ");
	int rid;
	scanf("%d", &rid);
	while(RendezvenyEllenorzes(rendezvenyek, rid) == 0){
		printf("Ez a rendezvény-azonosító nem létezik!\nRendezvény azonosytója: ");
		scanf("%d", &rid);
	}
	j.index=maxindex+1;
	j.ido=time(0);
	fwrite(&j, sizeof(struct Jelentkezes), 1, f);
	printf("Jelentkezés sikeres! Te vagy a %d. vendég!\n", j.index);
	fclose(f);
}

void UjRendezveny(){
	printf("Új Rendezvény azonosítója: ");
	int azon;
	//TODO: check if already exists
	scanf("%d", azon);
	FILE* f = fopen("rendezvenyek.data", "ab+");
	char str[15];
	sprintf(str, "%d\n", azon);
	fwrite(&str, sizeof(str), 1,f);
	fclose(f);
}

int RendezvenyEllenorzes(int rendezvenyek[], int n, int rendezveny){
	int i;
	for(i=0; i<n; i++){
		if(rendezvenyek[i] == rendezveny){
			return 1;
		}
	}
	return 0;
}

void JelBeolvas(struct Jelentkezes osszes[], int n){
	int i;
	for(i=0; i<n; i++){
		char* c_time = ctime(&osszes[i].ido);
		printf("%d %s %s %d %s", osszes[i].id, osszes[i].nev, osszes[i].mail, osszes[i].index, c_time);
	}
}

int VendegEllenorzes(struct Jelentkezes osszes[], int n, int vendeg){
	int i;
	for(i=0; i<n; i++){
		if(osszes[i].index == vendeg){
			return i;
		}
	}
	return -1;
}

int NoOfStructs(){
	struct stat st;
	stat("jelentkezesek.data", &st);
	int filesize = st.st_size;
	if(filesize == 568){
		return 0;
	}
	int structsize = sizeof(struct Jelentkezes);
	if(filesize%structsize != 0){
		printf("HIBA: Fájlméret nem osztható a struct méretével!");
		printf("File size: %d\n", filesize);
		printf("Struct size: %d\n", structsize);
		exit(1);
	}
	return filesize/structsize;
}

void JelentkezesModosit(struct Jelentkezes osszes[], int n){
	printf("Jelentkezés ID-je: ");
	int id;
	scanf("%d", &id);
	int index = VendegEllenorzes(osszes, n, id);
	while(index == -1){
		printf("Vendég ID nem létezik! Próbálja újra: ");
		scanf("%d", &id);
		int index = VendegEllenorzes(osszes, n, id);
	}
	//TODO: Egy rekord kiírása
	printf("Mit szeretnél módosítani? Név[N] E-mail[E] RendezvényID[R] Mégsem[X]");
	char c=getchar();
	c=toupper(c);
	char s[50];	
		switch(c){
			case 'N':
				printf("Új név: ");	
				fgets(s, 50, stdin);
				strcpy(osszes[index].nev, s);
				break;
			case 'E':
				printf("Új e-mail: ");
				fgets(s, 50, stdin);
				strcpy(osszes[index].mail, s);
				break;
			case 'R':
				printf("Új rendezvény: ");
				//TODO
			default:
				break;
		}
}

int main(){
	int b = 0;
	while(b==0){
		FILE* f = fopen("jelentkezesek.data", "rb");
		int i;
		int n = NoOfStructs();
		struct Jelentkezes osszes[n];
		for(i=0; i<n; i++){
			fread(&osszes[i], sizeof(struct Jelentkezes), 1, f);
		}
		fclose(f);
		int maxIndex  = osszes[n-1].index;
		FILE* fr = fopen("rendezvenyek.txt", "r");
		int lines = 0;
		int ch=1;
		while(ch != -1)
			{
			ch = fgetc(fr);
			if(ch == '\n')
			  {
				lines++;
			  }
			  //printf("%d\n", ch);
			}
		int rendezvenyek[lines];
		fclose(fr);
		//rewind(fr); //why the segfault?
		FILE* fr2 = fopen("rendezvenyek.txt", "r");
		i=0;
		int num;
		while(fscanf(fr2, "%d", &num) != -1){
			rendezvenyek[i] = num;
			//printf("%d\n", rendezvenyek[i]);
			i++;
		}	
		fclose(fr);
		printf("Üdvözlet a King of Stands nyilvántartásában! Lehetőségek:\nÚj fájl[U] Listázás[L] Jelentkezés módosítása[M] Kilépés[X]\n");
		char c=getchar();
		c=toupper(c);
		switch(c){
			case 'U':
				UjJelentkezes(maxIndex, rendezvenyek);
				break;
			case 'L':
				JelBeolvas(osszes, n);
				break;
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