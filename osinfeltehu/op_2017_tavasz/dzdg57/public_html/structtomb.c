#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

struct myStruct {
	int num;
	char ch;
};

void kiir_fopen_bin(char* fajlnev, struct myStruct str) {

	FILE * f;
	// http://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
	// ezert hasznalunk wb es nem w-t... nem szovegfajllal dolgozunk (nem egy txt)
	f = fopen(fajlnev, "wb");
	fwrite(str, sizeof(str[0]), sizeof(str)/sizeof(str[0]), f);
	fclose(f);
}

int main(int argc, char** argv) {
	// english abc #
	int n = 26;
	struct myStruct st[n];
	st[0].num = 0;
	st[0].ch = 'a';
	int i;
	for(i=1; i<n; i++) {
		st[i].num = st[i-1].num + 1;
		st[i].ch = st[i-1].ch + 1;
	}
	
	//tomb kiirasa fajlba
	char * fajlnev = "tomb.dat";
	if(!access(fajlnev, W_OK) == 0) {perror("Nincs jogod irni a fajlt\n"); exit(1);}
	kiir_fopen_bin(fajlnev, st);	
	
	for(i=0; i<n; i++) {
		printf("az %i. elem sorszama: %i, karakter: %c\n",i, st[i].num, st[i].ch);

	}
}
