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

//sz�veghossz
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
//karakter sz�ml�l�s
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
//sz�vegkeres�s


int main(){
	int f = open("coords.txt", O_RDWR|O_CREAT, S_IRUSR);	//f�jl megnyit�s �r�sra
	struct coords c; //L�trehoz�s
	printf("Coords merete: %i\n",sizeof(c)); //%i integert �ratunk ki
	strcpy(c.x,"01234"); //hova,mit
	strcpy(c.y,"56789"); //hova,mit
	write(/*Mibe*/f, /*Mit*/&c, /*Mekkor�t*/sizeof(c)); // vagy 10*sizeif(char)
	close(f); //f�jl bez�r�s

	int fRead = open("coords.txt",O_RDONLY, S_IRUSR);	//F�jln�v, mire nyitom meg (olvas�s only), nemtudjukmirej�
	char cRead;
	while(read(fRead, &cRead, 1)){		//beolvasunk mib�l, mibe, mennyit
		printf("%c", cRead);
	}
	printf("\n");
	close(fRead);
	return 0;
}