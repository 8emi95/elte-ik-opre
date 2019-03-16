#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // open, close, read, write
#include <time.h>

void f3 () {
	time_t mytime = time(NULL);
	printf ("Current local time and date: %s", ctime(&mytime));
}

void f5 (char* path) {
	int file = open(path, O_RDONLY); // szöveges fájl megnyitása olvasásra
	
	if (file) {
		char c; // karakterenkét olvassuk ki a fájl tartalmát a c változóba 
		
		while (read(file, &c, sizeof(c))) {
			printf("%c", c);
		}
		printf("\n");

		close(file); // végül bezárjuk a fájlt
	} else {
		printf("hiba\n");
	}
}

void f6 () {
	srand(time(NULL)); // starting value of rng
	
	int d1 = rand() % 6 + 1; // num between 1 - 6
	int d2 = rand() % 6 + 1; // num between 1 - 6
	
	printf("dice1:%i, dice2:%i, sum=%i\n", d1, d2, d1 + d2);
}

int main (int argc, char ** argv) {
	//f3();
	//f5("./text.txt");
	//f6();
	
	return 0;
}
