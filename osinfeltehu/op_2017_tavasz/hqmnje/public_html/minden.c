#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		//open, creat
#include <sys/types.h>	//open
#include <sys/stat.h>
#include <errno.h>		//perror, errno
#include <string.h>

//Parancsok
//	gcc minden.c
//	./a.out
struct coords{
	char x[5];
	char y[5];
};

//szöveghossz
int hossz(char* s){
	int i = 0;
	while(s[i++]);	//vagy (*s++ != 0)
	return i;
}
//karaktercsere
void* karakter_csere(char* miben, char* mit, char* mire){
	while (*miben){
		if(*miben == *mit){
			*miben = *mire;
		}
		*miben++;
	}
}
//karakter számlálás
int karakter_szamlalas(char* miben, char* mit){
	int sum;
	while (*miben){
		if(*miben == *mit){
			sum++;
		}
		*miben++;
	}
	return sum;
}
//szövegkeresés


int main(){
	int f = open("coords.txt", O_RDWR|O_CREAT, S_IRUSR);	//fájl megnyitás írásra
	struct coords c; //Létrehozás
	printf("Coords merete: %i\n",sizeof(c)); //%i integert íratunk ki
	strcpy(c.x,"01234"); //hova,mit
	strcpy(c.y,"56789"); //hova,mit
	write(/*Mibe*/f, /*Mit*/&c, /*Mekkorát*/sizeof(c)); // vagy 10*sizeif(char)
	close(f); //fájl bezárás

	int fRead = open("coords.txt",O_RDONLY, S_IRUSR);	//Fájlnév, mire nyitom meg (olvasás only), nemtudjukmirejó
	char cRead;
	while(read(fRead, &cRead, 1)){		//beolvasunk mibõl, mibe, mennyit
		printf("%c", cRead);
	}
	printf("\n");
	close(fRead);
	return 0;
}