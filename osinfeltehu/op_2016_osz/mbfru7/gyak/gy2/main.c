#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // open, close, read, write

#include <sys/stat.h> // stat
#include <unistd.h> // stat
#include <time.h> // ctime

/*
 * BEADANDÓK KIÍRÁSRA KERÜLTEK
 * GYAK: http://os.inf.elte.hu/~pzh5fq/
*/

/*
 * open   fopen(text)  fopen(bin)
 * close  fclose       fclose
 * read   fgets        fread
 * write  fputs        fwrite
 * lseek               fseek
*/

void fv (int handle) {
	char nev[] = "Nyitrai Gergő";
	write(handle, &nev, sizeof(nev));
}

int main (int argc, char ** argv) {
	/*
		// szöveges fájl megnyitása
		// open visszatérési értéke egy int "handle"
		int file = open("./text.txt", O_RDONLY);

		// karakterenként olvasunk a c nevű változóba
		char c;

		while (read(file, &c, sizeof(c))) {
			printf("%c", c);
		}

		close(file);
	*/

	/*
		int handle = open("./text.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
		fv(handle);
		close(handle);
	*/

	/*
		struct stat st;
		stat(argv[0],&st);  //fstat(file descriptor, stat structure)
		time_t t=st.st_mtime;
		printf("The last modification was: %s\n",ctime(&t)); //convert time_t to string
	*/

	/*
		srand(time(NULL)); // starting value of rng
		int r = rand() % 100; // num between 0-99
		printf("Random number %i\n", r);
	*/

	return 0;
}
